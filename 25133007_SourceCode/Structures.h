#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

const int MAX_YARD_CAPACITY = 4000;

enum class Status_Container {
  pre_gate = 1, // cho vao cong
  in_yard = 2,  // da duoc add vao trong bai
  released = 3  // da giai phong khoi bai
};

enum class Label_Container {
  DANGER = 1, // hang nguy hiem
  RF = 2,     // hang dong lanh
  GP = 3      // hang thuong
};

struct Container {
  Label_Container container_label; // nhãn container
  string container_id;             // id container
  Status_Container status;         // trạng thái container
  double gross_weight;             // khối lượng container
  string customs_declaration_no;   // mã tờ khai

  // Nạp chồng toán tử so sánh các container
  bool operator<(const Container &other) const {
    // top1. Ưu tiên theo cân nặng lớn hơn
    if (gross_weight != other.gross_weight)
      return gross_weight < other.gross_weight;
    // top2. Ưu tiên theo nhãn nghiêm trọng hơn
    if (container_label != other.container_label)
      return static_cast<int>(container_label) >
             static_cast<int>(other.container_label);
    // top3. Ưu tiên theo id nhỏ hơn
    return container_id > other.container_id;
  }
};

struct Vessel {
  list<Container> ContainerList;
};

// cấu trúc heap cẩu container, khi truyền vào các hàm phải có dấu tham chiếu &
struct priority_queue {
  Container *Data;  // Cấp phát trỏ trên heap
  int capacity;     // Sức chứa
  int current_size; // Số phần tử hiện có

  // Hàm tạo Constructor khởi tạo sức chứa 16 khi vừa khai báo
  priority_queue(int beginning_capacity = 16) {
    capacity = beginning_capacity;
    current_size = 0;
    Data = new Container[capacity];
  }

  // Tự giải phóng bộ nhớ
  ~priority_queue() { delete[] Data; }

  // Nhân đôi kích thước Sức chứa
  void resize() {
    capacity *= 2;
    Container *new_data = new Container[capacity];
    for (int i = 0; i < current_size; i++) {
      new_data[i] = Data[i];
    }

    delete[] Data;
    Data = new_data;
  }

  // Phần tử vừa được thêm vào nổi lên theo quy tắc của cây nhị phân hoàn chỉnh
  void sift_up(int i) {
    while (i > 0) {
      int parrent = (i - 1) / 2;
      if (Data[parrent] < Data[i]) {
        swap(Data[parrent], Data[i]);
        i = parrent;
      } else
        break;
    }
  }

  // Phần tử cuối bị thêm lên trên đầu (để thế chỗ phần tử vừa bị lấy ra từ trên
  // đầu) chìm xuống
  void sift_down(int i) {
    while (2 * i + 1 < current_size) {
      int left_child = 2 * i + 1;
      int right_child = 2 * i + 2;
      int more_priority_container_index = i;

      if (Data[more_priority_container_index] < Data[left_child]) {
        more_priority_container_index = left_child;
      }
      if (right_child < current_size &&
          Data[more_priority_container_index] < Data[right_child]) {
        more_priority_container_index = right_child;
      }

      if (more_priority_container_index != i) {
        swap(Data[i], Data[more_priority_container_index]);
        i = more_priority_container_index;
      } else
        break;
    }
  }

  bool empty() const { return current_size == 0; }
  int size() const { return current_size; }

  void push(const Container &new_exported_container) {
    if (current_size == capacity) {
      resize(); // Tự nở sức chứa khi đầy
    }
    Data[current_size] = new_exported_container;
    sift_up(current_size);
    current_size++;
  }

  Container top() const {
    if (empty()) {
      throw runtime_error("    *THONG BAO: Hang doi xuat khong dang rong! \n");
    }
    return Data[0];
  }

  void pop() {
    if (empty()) {
      cout << "    *THONG BAO: Hang doi xuat khau dang rong! \n";
      return;
    }
    Data[0] = Data[current_size - 1];
    current_size--;

    if (current_size > 0) {
      sift_down(0);
    }
  }
};

// Hàm tính số lượng container hiện có trong bãi trước khi tiến hành nhập
// container(s) ngoài cổng (đừng quan tâm hàm này)
int CountContainersInYard(const unordered_map<string, Vessel> &VesselMap);

// Hàm nhập container(s) ngoài cổng vào bãi
void AddContainer(unordered_map<string, Vessel> &VesselMap,
                  list<Container> &GateContainerQueues);

string labelToString(Label_Container label);

string statusToString(Status_Container status);

void ExportContainer(string &container_id,
                     unordered_map<string, Vessel> &VesselMap,
                     priority_queue &ExportQueue);

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue &ExportQueue);

void PrintOrderedExportedContainerQueue(priority_queue &ExportQueue);
