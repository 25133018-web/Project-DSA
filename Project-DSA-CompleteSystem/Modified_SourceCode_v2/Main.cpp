#include "Persistence.h"
#include "YardSystem.h"

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
