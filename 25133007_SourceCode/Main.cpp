#include "Structures.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

/* Hàm khởi tạo dữ liệu mẫu cho hàng đợi ngoài cổng
void InitSampleData(list<Container> &GateContainerQueues) {
  GateContainerQueues.push_back({Label_Container::GP, "MAEU1001",
Status_Container::pre_gate, 25.5, "100000000001"});
  GateContainerQueues.push_back({Label_Container::DANGER, "MAEU1002",
Status_Container::pre_gate, 28.0, "100000000001"});
  GateContainerQueues.push_back({Label_Container::RF, "MAEU1003",
Status_Container::pre_gate, 30.2, "100000000002"});
  GateContainerQueues.push_back({Label_Container::GP, "COSU2001",
Status_Container::pre_gate, 30.2, "100000000003"});
  GateContainerQueues.push_back({Label_Container::DANGER, "COSU2002",
Status_Container::pre_gate, 30.2, "100000000003"});
  GateContainerQueues.push_back({Label_Container::RF, "SGN3001",
Status_Container::pre_gate, 19.8, "100000000004"});
}*/

int main() {

  unordered_map<string, Vessel> VesselMap;
  list<Container> GateContainerQueues;
  priority_queue ExportQueue;

  /* Nạp sẵn dữ liệu container đang xếp hàng ở cổng
  InitSampleData(GateContainerQueues);*/

  // Đầu chương trình gọi hàm nhập cảng các container(s) ngoài cổng
  AddContainer(VesselMap, GateContainerQueues);

  // Gọi hàm cho phép người dùng tương tác với hệ thống
  Interact(VesselMap, ExportQueue);

  return 0;
}
