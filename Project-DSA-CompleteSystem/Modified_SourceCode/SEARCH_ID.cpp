#include "YardSystem.h"

void SEARCH_ID(const HashTable &yardHashTable) {
  string target_id;
  cout << "\n==========================================";
  cout << "\nNhap vao ID Container can tim kiem: ";
  cin >> target_id;

  // Gọi hàm tìm kiếm trong bảng băm (bên trong chính là vòng lặp duyệt của bạn
  // mc1)
  Container *found_container = yardHashTable.search(target_id);

  if (found_container != nullptr) {
    cout << "Thong tin container mang ID : " << target_id;
    displayContainer(*found_container);
  } else {
    cout << "\nKhong tim thay Container mang ID " << target_id << "\n";
  }
}
