#include "YardSystem.h"
#include <cctype>

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
                  list<Container> &GateContainerQueues,
                  HashTable &yardHashTable, ContainerTrie &yardTrie,
                  DSU &yardDSU,
                  unordered_map<string, Container *> &containerLookup) {

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

    // Đồng bộ hóa dữ liệu sang HashTable MC1:
    yardHashTable.insert(container_in_yard);

    // Đồng bộ dữ liệu vô cây Trie CN1:
    // Lấy con trỏ container vừa lưu cắm vào cây trie để gợi ý tiền tố
    yardTrie.insert(&(VesselMap[id_voyage_check].ContainerList.back()));

    // Đồng bộ dữ liệu sang CN2:
    // Nạp ID vào DSU và lưu thông tin vào danh sách mã tờ khai
    containerLookup[container_in_yard.container_id] =
        &(VesselMap[id_voyage_check].ContainerList.back());
    yardDSU.addContainer(container_in_yard.container_id);

    GateContainerQueues
        .pop_front(); // Xóa container vừa thêm vào bãi ra khỏi hàng đợi
  }

  // Thông báo đã hoàn thành việc thêm
  cout << "   *Da them thanh cong " << imported_container_quantity
       << " vao bai\n";
  cout << "   *Hang doi ngoai cong con lai " << GateContainerQueues.size()
       << " container(s) \n";
}
