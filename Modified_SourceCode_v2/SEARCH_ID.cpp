#include "YardSystem.h"

void SEARCH_ID(const HashTable &yardHashTable, ContainerTrie &yardTrie) {
  string target_id;
  cout << "\n==========================================";
  cout << "\nNhap vao ID Container can tim kiem: ";
  cin >> target_id;

  // Chuẩn hóa ID sang chữ In Hoa
  target_id = normalizeID(target_id);

  if (target_id.length() > 11) {
    cout << "Loi. Ma container phai gom dung 11 ky tu!\n";
    return;
  }
  if (target_id.length() < 2) {
    cout << "Loi: Tien to nhap vao phai tu 2 ky tu tro len\n";
    return;
  }

  // Trường hợp 1: Nhập đúng 11 ký tự -> Tra cứu chính xác qua HashTable
  if (target_id.length() == 11) {
    Container *found_container = yardHashTable.search(target_id);

    if (found_container != nullptr) {
      cout << "\nThong tin container mang ID : " << target_id;
      displayContainer(*found_container);
    } else {
      cout << "\nKhong tim thay Container mang ID " << target_id << "\n";
    }
    return;
  }

  // Trường hợp 2: Nhập từ 2 đến 10 ký tự -> Gợi ý tiền tố qua Trie
  vector<Container *> suggestions = yardTrie.searchbyprefix(target_id);
  if (suggestions.empty()) {
    cout << "\nKhong tim thay container nao co tien to " << target_id
         << " trong bai!\n";
    return;
  }

  cout << "\n[GOI Y] Tim thay " << suggestions.size()
       << " container khop tien to " << target_id << ":\n";

  int display_limit = min(static_cast<int>(suggestions.size()), 15);
  for (int i = 0; i < display_limit; i++) {
    cout << suggestions[i]->container_id;
    if (i < display_limit - 1) {
      cout << " | ";
    }
  }
  if (suggestions.size() > 15) {
    cout << " | ... (va con " << (suggestions.size() - 15)
         << " container khac)";
  }
  cout << "\n";

  // Thao tác tiếp nối: Cho phép gõ luôn ID từ danh sách trên để xem chi tiết
  cout << "\nNhap ma ID tu danh sach de xem chi tiet (hoac 0 de bo qua): ";
  cin >> target_id;
  target_id = normalizeID(target_id);

  if (target_id == "0") {
    return;
  }

  Container *found_container = yardHashTable.search(target_id);
  if (found_container != nullptr) {
    cout << "\nThong tin container mang ID : " << target_id;
    displayContainer(*found_container);
  } else {
    cout << "\nKhong tim thay Container mang ID " << target_id << "\n";
  }
}
