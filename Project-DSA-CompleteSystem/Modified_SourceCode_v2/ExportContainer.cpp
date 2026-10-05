#include "Container.h"
#include "YardSystem.h"
#include <cctype>
#include <iomanip>
#include <sstream>

void ExportContainer(string &container_id,
                     unordered_map<string, Vessel> &VesselMap,
                     priority_queue &ExportQueue, HashTable &yardHashTable,
                     ContainerTrie &yardTrie,
                     unordered_map<string, Container *> &containerLookup) {

  // Tách chữ trong ID (mã chuyến tàu)
  string id_voyage_check = "";
  for (char c : container_id) {
    if (isalpha(c)) {
      id_voyage_check += c;
    } else {
      break;
    }
  }

  // Không tiếp tục tìm kiếm nếu không tồn tại mã chuyến tàu => cũng không tồn
  // tại container có ID chứa mã chuyến tàu đó
  if (id_voyage_check.empty() ||
      VesselMap.find(id_voyage_check) == VesselMap.end()) {
    cout << "THONG BAO: Khong tim thay tau tuong ung cho ma ID " << container_id
         << "\n";
    return;
  }

  // Bắt đầu tìm kiếm container và xuất ra danh sách cẩu
  auto &list_of_the_exported_container =
      VesselMap[id_voyage_check]
          .ContainerList; // Ghi lại cái danh sách container có mã chuyến tàu
                          // tương ứng
  bool Successfully_found = false;

  for (auto temp = list_of_the_exported_container.begin();
       temp != list_of_the_exported_container.end();
       temp++) { // Thao tác duyệt tìm container cần xuất
    if (temp->container_id == container_id) { // Nếu tìm được
      Container the_exported_container_found =
          *temp; // tạo biến tạm ghi lại thông tin container đó
      the_exported_container_found.status =
          Status_Container ::released; // đổi trang thái biến tạm
      ExportQueue.push(
          the_exported_container_found); // cho biến tạm vào danh sách cẩu
      yardHashTable.remove(container_id);
      yardTrie.remove(container_id);
      containerLookup.erase(container_id);
      list_of_the_exported_container.erase(
          temp); // xóa container đã tìm được ra khỏi VesselMap

      Successfully_found = true;
      break;
    }
  }

  if (Successfully_found) {
    cout << "    *Da xuat thanh cong container co ID " << container_id
         << " ra khoi bai\n";
  } else {
    cout << "THONG BAO: Khong tim thay container co ID " << container_id
         << " trong danh sach cua tau co ma chuyen tau " << id_voyage_check
         << "\n";
  }
}

void PrintOrderedExportedContainerQueue(priority_queue &ExportQueue) {
  if (ExportQueue.empty()) {
    cout << "\n[THONG BAO] Hang doi cau hien dang trong. Chua co container nao duoc xuat!\n";
    return;
  }

  cout << "\n========================================================================================\n";
  cout << "|                   DANH SACH DIEU PHOI CAU CONTAINER LEN TAU (MAX-HEAP)                |\n";
  cout << "========================================================================================\n";
  cout << left << setw(5)  << "STT"
       << "| " << setw(13) << "MA CONTAINER"
       << "| " << setw(13) << "TRONG LUONG"
       << "| " << setw(18) << "LOAI HANG"
       << "| " << setw(15) << "TRANG THAI"
       << "| " << setw(16) << "DO UU TIEN CAU" << "\n";
  cout << "-----+--------------+--------------+-------------------+----------------+-----------------\n";

  int ordinal_number = 1;
  while (!ExportQueue.empty()) {
    Container top_c = ExportQueue.top();
    string priority_tag = (ordinal_number == 1) ? "[MAX PRIORITY]" : "THU TU CAU";

    ostringstream weight_ss;
    weight_ss << fixed << setprecision(2) << top_c.gross_weight << " tan";

    cout << left << setw(5)  << (to_string(ordinal_number++) + ".")
         << "| " << setw(13) << top_c.container_id
         << "| " << setw(13) << weight_ss.str()
         << "| " << setw(18) << labelToString(top_c.container_label)
         << "| " << setw(15) << statusToString(top_c.status)
         << "| " << setw(16) << priority_tag << "\n";

    ExportQueue.pop();
  }
  cout << "----------------------------------------------------------------------------------------\n";
  cout << "(*) Ghi chu: Container co trong luong lon hon duoc cau truoc de dam bao trong tam tau.\n\n";
}
