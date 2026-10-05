#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/HashTable.h"
#include <iostream>
using namespace std;

void printTestResult(const string &message, bool passed) {
  cout << message << " -> Ket qua test: "
       << (passed ? "true" : "false") << "\n";
}

int main() {
  HashTable table;

  cout << "--- Test HashTable ---\n";

  // 1. Them container hop le
  Container c1 = {Label_Container::GP, "ABCD1234567", Status_Container::in_yard,
                  12.5, "DECL-1"};
  table.insert(c1);
  printTestResult("1) Them container hop le",
                  table.search("ABCD1234567") != nullptr);

  // 2. Tim kiem container bang ID dung
  printTestResult("2) Tim bang ID dung",
                  table.search("ABCD1234567") != nullptr);

  // 3. Chuan hoa ID viet thuong truoc khi tim, giong cach dung trong ung dung
  printTestResult("3) Tim bang ID viet thuong",
                  table.search(normalizeID("abcd1234567")) != nullptr);

  // 4. Xoa container bang ID viet thuong
  bool removed = table.remove(normalizeID("abcd1234567"));
  printTestResult("4) Xoa container bang ID viet thuong", removed);

  // 5. Tim kiem sau khi xoa
  printTestResult("5) Tim sau khi xoa",
                  table.search("ABCD1234567") == nullptr);

  // 6. Xoa lai ID khong ton tai
  bool removedAgain = table.remove("ABCD1234567");
  printTestResult("6) Xoa ID khong ton tai", !removedAgain);

  // 7. Them lai container
  Container c2 = {Label_Container::DANGER, "ABCD1234567", Status_Container::in_yard,
                  18.0, "DECL-2"};
  table.insert(c2);
  printTestResult("7) Them lai container sau khi xoa",
                  table.search("ABCD1234567") != nullptr);

  // 8. Tim kiem bang ID viet thuong sau khi them lai
  printTestResult("8) Tim bang ID viet thuong sau khi them lai",
                  table.search(normalizeID("abcd1234567")) != nullptr);

  // 9. Tim ID khong ton tai
  printTestResult("9) Khong tim thay ID khong ton tai",
                  table.search("ZZZZ9999999") == nullptr);

  // 10. Tim ID ngan
  printTestResult("10) Khong tim thay ID ngan",
                  table.search("SHORT") == nullptr);

  return 0;
}
