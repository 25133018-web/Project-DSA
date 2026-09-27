#include "Structures.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

// Hàm đếm số container đang có trong bãi
int CountContainersInYard(const unordered_map<string, Vessel> &VesselMap) {
  int total = 0;
  for (const auto &pair : VesselMap) {
    total += pair.second.ContainerList.size();
  }
  return total;
}

// Hàm thêm container từ hàng đợi ngoài cổng vào trong bãi
void AddContainer(unordered_map<string, Vessel> &VesselMap,
                  list<Container> &GateContainerQueues) {

  int current_container_quantity_in_yard =
      CountContainersInYard(VesselMap); // Số lượng container(s) hiện trong bãi

  // Kiểm tra số lượng container(s) ngoài cổng
  if (GateContainerQueues.empty()) {
    cout << "THONG BAO: KHONG CO CONTAINER DE THEM VAO \n";
    return;
  }

  int available_space =
      MAX_YARD_CAPACITY -
      current_container_quantity_in_yard; // Sức chứa khả dụng của bãi

  // Các thông báo giao diện nhập liệu đơn giản
  cout << "TIEN HANH NHAP CONTAINER(S) VAO TRONG BAI \n";
  cout << "So luong container(s) dang doi o cong: "
       << GateContainerQueues.size() << endl;
  cout << "Nhap so luong containers duoc phep thong qua (gioi han "
       << available_space << " container(s)): ";

  // Nhập số lượng container muốn thêm
  int imported_container_quantity = 0;
  cin >> imported_container_quantity;

  // Kiểm tra số lượng container(s) thông qua so với khả dụng bãi và lượng hàng
  // đợi
  if (imported_container_quantity > available_space) { // So sánh với bãi
    cout << "Khong du khong gian cho viec them " << imported_container_quantity
         << " container(s)\n";
    return;
  }
  if (imported_container_quantity < 0) { // Kiểm tra không âm
    cout << "Nhap sai (so luong container(s) them vao khong duoc la so am!)\n";
    return;
  }
  if (imported_container_quantity >
      (int)GateContainerQueues.size()) { // So sánh với hàng đợi
    cout << "Ngoai cong chi co " << GateContainerQueues.size()
         << " containers. He thong se lay toi da so container dang co!\n";
    imported_container_quantity = GateContainerQueues.size();
  }

  // Tiến hành thêm vào bãi
  for (int i = 0; i < imported_container_quantity; i++) {
    // Tách chữ trong ID (mã chuyến tàu)
    string id_voyage_check = "";
    for (char c : GateContainerQueues.front().container_id) {
      if (isalpha(c)) {
        id_voyage_check += c;
      } else {
        break;
      }
    }

    // Nhập Container vào VesselMap và cập nhật trạng thái
    Container container_in_yard = GateContainerQueues.front();
    container_in_yard.status = Status_Container::in_yard;
    VesselMap[id_voyage_check].ContainerList.push_back(container_in_yard);
    GateContainerQueues.pop_front();
  }

  // Thông báo đã hoàn thành việc thêm
  cout << "   *Da them thanh cong " << imported_container_quantity
       << " vao bai\n";
  cout << "   *Hang doi ngoai cong con lai " << GateContainerQueues.size()
       << " container(s) \n";
}
