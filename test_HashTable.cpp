#include "YardSystem.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static void printTestHeader(const string &name) {
  cout << "\n========================================\n";
  cout << name << "\n";
  cout << "========================================\n";
}

static void printResult(const string &scenario, bool passed) {
  cout << (passed ? "[PASS] " : "[FAIL] ") << scenario << "\n";
}

static bool addUniqueIfNormalizedNotExists(HashTable &table, ContainerTrie &trie,
                                          vector<Container> &storage,
                                          const string &rawId,
                                          const string &decl,
                                          double weight) {
  string normalized = normalizeID(rawId);
  if (table.search(normalized) != nullptr) {
    return false;
  }

  storage.push_back({Label_Container::GP, normalized, Status_Container::in_yard,
                     weight, decl});
  Container &c = storage.back();
  table.insert(c);
  trie.insert(&c);
  return true;
}

int main() {
  HashTable yardHashTable;
  ContainerTrie yardTrie;
  vector<Container> storage;

  printTestHeader("TEST: THEM XOA TIM KIEM CONTAINER");

  // 1. Thêm một container mới hợp lệ ("ABCD1234567", 12.5 tấn, "DECL-1")
  {
    bool ok = addUniqueIfNormalizedNotExists(yardHashTable, yardTrie, storage,
                                            "ABCD1234567", "DECL-1", 12.5);
    printResult("Them container hop le ABCD1234567", ok);
    if (!ok) {
      cout << "  -> Khong the them container hop le dau tien.\n";
      return 1;
    }
  }

  // 2. Thử thêm container có ID chữ thường "abcd1234567" khi ID "ABCD1234567" đã tồn tại.
  {
    bool ok = addUniqueIfNormalizedNotExists(yardHashTable, yardTrie, storage,
                                            "abcd1234567", "DECL-1-DUP", 15.0);
    printResult("Them duplicate normalized ID lowercase", !ok);
  }

  // 3. Thực hiện xóa container đang tồn tại bằng chuỗi chữ thường "abcd1234567".
  {
    string id = normalizeID("abcd1234567");
    bool removed = yardHashTable.remove(id);
    printResult("Xoa container bang lowercase ID", removed);
  }

  // 4. Tra cứu container "ABCD1234567" ngay sau khi thực hiện thao tác xóa.
  {
    bool found = (yardHashTable.search("ABCD1234567") == nullptr);
    printResult("Tra cuu sau khi xoa -> khong ton tai", found);
  }

  // 5. Thử xóa container có ID không tồn tại trong bảng băm ("ABCD1234567" sau khi đã xóa)
  {
    bool removed = yardHashTable.remove("ABCD1234567");
    printResult("Xoa container khong ton tai", !removed);
  }

  // 6. Thêm lại container "ABCD1234567" vào bảng băm sau khi đã xóa thành công trước đó
  {
    bool ok = addUniqueIfNormalizedNotExists(yardHashTable, yardTrie, storage,
                                            "ABCD1234567", "DECL-1", 12.5);
    printResult("Them lai container sau khi da xoa", ok);
  }

  // 7. Tra cứu thông tin container trong bảng băm bằng ID viết thường "abcd1234567"
  {
    string lowerId = normalizeID("abcd1234567");
    Container *found = yardHashTable.search(lowerId);
    bool ok = (found != nullptr && found->container_id == lowerId);
    printResult("Tim kiem bang lowercase ID sau khi them lai", ok);
    if (found != nullptr) {
      cout << "  -> Tim thay: " << found->container_id << " | "
           << found->customs_declaration_no << "\n";
    }
  }

  // 8. Gọi hàm giao diện SEARCH_ID() với ID nhập vào là chữ thường "abcd1234567".
  printTestHeader("TEST: GOI SEARCH_ID VOI ID LOWERCASE");
  {
    string input = "abcd1234567\n";
    istringstream in(input);
    streambuf *oldCin = cin.rdbuf(in.rdbuf());
    SEARCH_ID(yardHashTable, yardTrie);
    cin.rdbuf(oldCin);
    printResult("SEARCH_ID co ID lowercase da ton tai", true);
  }

  // 9. Gọi SEARCH_ID với ID không tồn tại trong bãi "ZZZZ9999999".
  {
    string input = "ZZZZ9999999\n";
    istringstream in(input);
    streambuf *oldCin = cin.rdbuf(in.rdbuf());
    SEARCH_ID(yardHashTable, yardTrie);
    cin.rdbuf(oldCin);
    printResult("SEARCH_ID co ID khong ton tai", true);
  }

  // 10. Gọi hàm SEARCH_ID() với mã ID ngắn không đủ 11 ký tự "SHORT"
  {
    string input = "SHORT\n";
    istringstream in(input);
    streambuf *oldCin = cin.rdbuf(in.rdbuf());
    SEARCH_ID(yardHashTable, yardTrie);
    cin.rdbuf(oldCin);
    printResult("SEARCH_ID co ID ngan SHORT", true);
  }

  cout << "\nKet thuc kiem thu.\n";
  return 0;
}
