#include <list>
#include <queue>
#include <string>
#include <unordered_map>

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
                     priority_queue<Container> &ExportQueue);

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue<Container> &ExportQueue);

void PrintOrderedExportedContainerQueue(priority_queue<Container> &ExportQueue);