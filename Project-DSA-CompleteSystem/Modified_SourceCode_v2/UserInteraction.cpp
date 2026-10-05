#include "YardSystem.h"
#include <iomanip>

// Hàm hiển thị Menu chính của hệ thống điều phối
void InMenuChinh() {
  cout << "\n========================================================================\n";
  cout << "|       HE THONG DIEU PHOI & QUAN LY CONTAINER CANG BIEN QUOC TE       |\n";
  cout << "========================================================================\n";
  cout << "  [1] Tra cuu Container (MC1: HashTable O(1) / CN1: Trie Goi y tien to)\n";
  cout << "  [2] Xuat Container khoi bai (Day vao hang doi cau tau)\n";
  cout << "  [3] Xem danh sach cau container len tau (MC2: Max-Heap Priority Queue)\n";
  cout << "  [4] Quan ly nhom to khai hai quan (CN2: Disjoint Set Union - DSU)\n";
  cout << "  [0] Ket thuc ca lam viec & Luu du lieu (Persistence Save & Exit)\n";
  cout << "------------------------------------------------------------------------\n";
  cout << "Nhap lua chon cua ban (0 - 4): ";
}

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue &ExportQueue, DSU &yardDSU,
              unordered_map<string, Container *> &containerLookup,
              HashTable &yardHashTable, ContainerTrie &yardTrie) {

  // Gom nhóm các container có cùng mã tờ khai vào DSU khi khởi tạo
  groupContainersByDeclaration(VesselMap, yardDSU, containerLookup);

  while (true) {
    InMenuChinh();
    int choice = -1;

    // Bắt lỗi nếu người dùng nhập ký tự không phải số
    if (!(cin >> choice)) {
      cin.clear();
      cin.ignore(10000, '\n');
      cout << "\n[!] Lua chon khong hop le! Vui long nhap so tu 0 den 4.\n";
      continue;
    }

    // [1] Tra cứu thông tin container (MC1 & CN1)
    if (choice == 1) {
      SEARCH_ID(yardHashTable, yardTrie);
    }
    // [2] Thực thi lệnh xuất khẩu container
    else if (choice == 2) {
      string commandded_id;
      cout << "\nNhap vao ID Container can xuat khoi bai: ";
      cin >> commandded_id;
      commandded_id = normalizeID(commandded_id);

      ExportContainer(commandded_id, VesselMap, ExportQueue, yardHashTable,
                      yardTrie, containerLookup);
    }
    // [3] Cẩu container(s) trong danh sách xuất khẩu có thứ tự lên tàu (MC2)
    else if (choice == 3) {
      PrintOrderedExportedContainerQueue(ExportQueue);
    }
    // [4] Chức năng 2: Quản lý nhóm container theo tờ khai (CN2 - DSU)
    else if (choice == 4) {
      int sub_choice = -1;
      while (sub_choice != 0) {
        cout << "\n==================================================\n";
        cout << "|     QUAN LY NHOM CONTAINER THEO TO KHAI (DSU)   |\n";
        cout << "==================================================\n";
        cout << "  [1] Tra cuu cac container cung nhom to khai\n";
        cout << "  [2] Gop 2 nhom container (Union to khai)\n";
        cout << "  [0] Quay lai Menu chinh\n";
        cout << "--------------------------------------------------\n";
        cout << "Chon thao tac (0 - 2): ";
        if (!(cin >> sub_choice)) {
          cin.clear();
          cin.ignore(10000, '\n');
          sub_choice = -1;
          cout << "\n[!] Lua chon khong hop le!\n";
          continue;
        }

        if (sub_choice == 1) {
          string targetID;
          cout << "Nhap ID container muon tra cuu: ";
          cin >> targetID;
          targetID = normalizeID(targetID);
          XuatThongTinCungMaToKhai(yardDSU, containerLookup, targetID);
        } else if (sub_choice == 2) {
          gopNhomContainer(yardDSU, containerLookup);
        } else if (sub_choice != 0) {
          cout << "\n[!] Lua chon khong hop le!\n";
        }
      }
    }
    // [0] Kết thúc ca làm việc
    else if (choice == 0) {
      cout << "\n[HE THONG] Dang dong ca lam viec va luu du lieu ra o dia...\n";
      break;
    }
    // Lựa chọn số không hợp lệ
    else {
      cout << "\n[!] Lua chon khong hop le! Vui long nhap so tu 0 den 4.\n";
    }
  }
}
