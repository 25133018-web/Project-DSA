#include "Persistence.h"
#include "YardSystem.h"

/* Hàm khởi tạo dữ liệu mẫu dự phòng cho hàng đợi ngoài cổng
void InitSampleData(list<Container> &GateContainerQueues) {
  GateContainerQueues.push_back({Label_Container::GP, "COSU2001111",
                                 Status_Container::pre_gate, 25.5,
                                 "100000000001"});
  GateContainerQueues.push_back({Label_Container::DANGER, "MAEU1002111",
                                 Status_Container::pre_gate, 28.0,
                                 "100000000001"});
  GateContainerQueues.push_back({Label_Container::RF, "MAEU1003123",
                                 Status_Container::pre_gate, 30.2,
                                 "100000000002"});
  GateContainerQueues.push_back({Label_Container::GP, "COSU2001321",
                                 Status_Container::pre_gate, 30.2,
                                 "100000000003"});
  GateContainerQueues.push_back({Label_Container::DANGER, "COSU2002456",
                                 Status_Container::pre_gate, 30.2,
                                 "100000000003"});
  GateContainerQueues.push_back({Label_Container::RF, "SGNG3001458",
                                 Status_Container::pre_gate, 19.8,
                                 "100000000004"});
}*/

int main() {

  unordered_map<string, Vessel> VesselMap;
  list<Container> GateContainerQueues;
  priority_queue ExportQueue;
  HashTable yardHashTable;
  ContainerTrie yardTrie;
  DSU yardDSU;
  unordered_map<string, Container *> containerLookup;

  const string database_file =
      "containers_database.csv"; // lưu lại tên file database

  // [TẦNG PERSISTENCE] 1. Nạp dữ liệu lúc khởi động (Load at Startup)
  if (!LoadContainersFromCSV(database_file, VesselMap, GateContainerQueues,
                             yardHashTable, yardTrie, yardDSU,
                             containerLookup)) {
    cout << "[PERSISTENCE] Chua co file '" << database_file << "'\n";
    // InitSampleData(GateContainerQueues);
  }

  // Nếu ngoài cổng có container đang chờ thì hỏi nhập bãi
  if (!GateContainerQueues.empty()) {
    AddContainer(VesselMap, GateContainerQueues, yardHashTable, yardTrie,
                 yardDSU, containerLookup);
  } else {
    cout << "\n[CONG CANG] Hien khong co container nao dang cho o cong.\n";
  }

  // Gọi hàm cho phép người dùng tương tác với hệ thống
  Interact(VesselMap, ExportQueue, yardDSU, containerLookup, yardHashTable,
           yardTrie);

  // [TẦNG PERSISTENCE] 2. Tự động lưu dữ liệu ra file đĩa khi kết thúc (Save on
  // Exit)
  SaveContainersToCSV(database_file, VesselMap, GateContainerQueues);

  return 0;
}
