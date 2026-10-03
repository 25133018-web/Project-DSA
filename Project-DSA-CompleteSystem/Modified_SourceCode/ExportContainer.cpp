#include "YardSystem.h"
#include <cctype>

void ExportContainer(string &container_id,
                     unordered_map<string, Vessel> &VesselMap,
                     priority_queue &ExportQueue) {

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

  cout << "Danh sach container(s) duoc cau len tau:\n";
  int ordinal_number = 1;
  while (!ExportQueue.empty()) {
    cout << ordinal_number++ << ". ID: " << ExportQueue.top().container_id
         << " --- Khoi luong: " << ExportQueue.top().gross_weight << "KG"
         << " --- Trang thai: " << statusToString(ExportQueue.top().status)
         << " --- Loai hang: "
         << labelToString(ExportQueue.top().container_label) << "\n";
    ExportQueue.pop();
  }
}
