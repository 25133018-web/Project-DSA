#pragma once

#include "Container.h"
#include <list>
#include <stdexcept>
#include <utility>

using namespace std;

// cấu trúc heap cẩu container, khi truyền vào các hàm phải có dấu tham chiếu &
class priority_queue {
private:
  Container *Data;  // Cấp phát trỏ trên heap
  int capacity;     // Sức chứa
  int current_size; // Số phần tử hiện có

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

public:
  // Hàm tạo Constructor khởi tạo sức chứa 16 khi vừa khai báo
  priority_queue(int beginning_capacity = 16) {
    capacity = beginning_capacity;
    current_size = 0;
    Data = new Container[capacity];
  }

  // Tự giải phóng bộ nhớ
  ~priority_queue() { delete[] Data; }

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
    if (empty()) { // Ném lỗi khi hàng đợi rỗng
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
