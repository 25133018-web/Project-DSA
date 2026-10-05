#pragma once

#include "Container.h"
#include "DSU.h"
#include "HashTable.h"
#include "Trie.h"
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

// Chuyển chuỗi sang Label_Container
inline Label_Container stringToLabel(const string &s) {
  if (s == "DANGER" || s == "1")
    return Label_Container::DANGER;
  if (s == "RF" || s == "2")
    return Label_Container::RF;
  return Label_Container::GP;
}

// Chuyển chuỗi sang Status_Container
inline Status_Container stringToStatus(const string &s) {
  if (s == "pre_gate" || s == "1")
    return Status_Container::pre_gate;
  if (s == "in_yard" || s == "2")
    return Status_Container::in_yard;
  if (s == "released" || s == "3")
    return Status_Container::released;
  return Status_Container::pre_gate;
}

// Chuyển enum sang chuỗi để ghi file
inline string statusToCode(Status_Container status) {
  switch (status) {
  case Status_Container::pre_gate:
    return "pre_gate";
  case Status_Container::in_yard:
    return "in_yard";
  case Status_Container::released:
    return "released";
  default:
    return "pre_gate";
  }
}

inline string labelToCode(Label_Container label) {
  switch (label) {
  case Label_Container::DANGER:
    return "DANGER";
  case Label_Container::RF:
    return "RF";
  case Label_Container::GP:
    return "GP";
  default:
    return "GP";
  }
}

// ==========================================
// TẦNG PERSISTENCE (LƯU TRỮ BỀN VỮNG - CSV)
// ==========================================

// 1. NẠP DỮ LIỆU KHI KHỞI ĐỘNG (Load at Startup)
// Phân loại tự động: 'in_yard' -> nạp thẳng vào bãi & các cấu trúc giải thuật;
// 'pre_gate' -> đưa vào hàng đợi cổng
inline bool LoadContainersFromCSV(
    const string &filename, unordered_map<string, Vessel> &VesselMap,
    list<Container> &GateContainerQueues, HashTable &yardHashTable,
    ContainerTrie &yardTrie, DSU &yardDSU,
    unordered_map<string, Container *> &containerLookup) {

  ifstream fin(filename);
  if (!fin.is_open()) {
    return false;
  }

  string line;
  // Bỏ qua dòng tiêu đề (header)
  if (!getline(fin, line)) {
    fin.close();
    return false;
  }

  int yardCount = 0;
  int gateCount = 0;

  while (getline(fin, line)) {
    if (line.empty())
      continue;

    stringstream ss(line);
    string labelStr, idStr, statusStr, weightStr, declStr;

    // Đọc từng trường phân tách bởi dấu phẩy
    if (getline(ss, labelStr, ',') && getline(ss, idStr, ',') &&
        getline(ss, statusStr, ',') && getline(ss, weightStr, ',') &&
        getline(ss, declStr, ',')) {

      // Xóa khoảng trắng thừa hoặc ký tự \r từ Windows line endings nếu có
      if (!declStr.empty() && declStr.back() == '\r') {
        declStr.pop_back();
      }

      Container c;
      c.container_label = stringToLabel(labelStr);
      c.container_id = normalizeID(idStr);
      c.status = stringToStatus(statusStr);
      try {
        c.gross_weight =
            stod(weightStr); // stod là viết tắt của string to double
      } catch (...) {        // ... nghĩa là bắt mọi loại lỗi
        c.gross_weight =
            200.0; // Cho khối lượng mặc định là 200.0 nếu dữ liệu bị lỗi
      }
      c.customs_declaration_no = declStr;

      // ========================================================
      // PHÂN LOẠI THÔNG MINH DỰA VÀO CỘT STATUS
      // ========================================================
      if (c.status == Status_Container::in_yard) {
        // 1. Tách mã chuyến tàu (các chữ cái đầu trong ID)
        string voyage = "";
        for (char ch : c.container_id) {
          if (isalpha(static_cast<unsigned char>(ch))) {
            voyage += ch;
          } else {
            break;
          }
        }

        // 2. Nạp vào danh sách bãi của tàu tương ứng
        VesselMap[voyage].ContainerList.push_back(c);
        Container *ptr = &(VesselMap[voyage].ContainerList.back());

        // 3. Tái thiết lập (Hydration) các bộ máy giải thuật trên RAM:
        yardHashTable.insert(c);              // MC1: Bảng băm tra cứu O(1)
        yardTrie.insert(ptr);                 // CN1: Cây tiền tố gợi ý tìm kiếm
        yardDSU.addContainer(c.container_id); // CN2: Quản lý liên thông tờ khai
        containerLookup[c.container_id] = ptr; // Bản đồ tra cứu địa chỉ RAM

        yardCount++;
      } else {
        // Mặc định hoặc pre_gate -> Xếp hàng đợi ngoài cổng
        c.status = Status_Container::pre_gate;
        GateContainerQueues.push_back(c);
        gateCount++;
      }
    }
  }

  fin.close();
  cout << "[PERSISTENCE] Da nap thanh cong tu file '" << filename << "':\n";
  cout << "  -> " << yardCount
       << " container dang trong bai (In Yard) da duoc nap vao HashTable, "
          "Trie, DSU.\n";
  cout << "  -> " << gateCount
       << " container dang cho ngoai cong (Pre Gate).\n";
  return true;
}

// 2. GHI DỮ LIỆU KHI THOÁT / LƯU (Save on Exit)
inline bool SaveContainersToCSV(const string &filename,
                                const unordered_map<string, Vessel> &VesselMap,
                                const list<Container> &GateContainerQueues) {
  ofstream fout(filename);
  if (!fout.is_open()) {
    cerr << "[PERSISTENCE LOI] Khong the mo file '" << filename
         << "' de ghi du lieu!\n"; // cerr giống với cout nhưng khi chương trình
                                   // bị lỗi cout sẽ bị lỗi theo, có thể không
                                   // chạy, còn lệnh cerr gặp lỗi vẫn chạy
    return false;
  }

  // Ghi header
  fout << "label,id,status,weight,declaration\n";

  int count = 0;

  // Ghi các container hiện đang lưu trong bãi (VesselMap)
  for (const auto &pair : VesselMap) {
    for (const auto &c : pair.second.ContainerList) {
      fout << labelToCode(c.container_label) << "," << c.container_id << ","
           << statusToCode(c.status) << "," << c.gross_weight << ","
           << c.customs_declaration_no << "\n";
      count++;
    }
  }

  // Ghi các container còn đang chờ ngoài cổng (chưa nhập vào bãi)
  for (const auto &c : GateContainerQueues) {
    fout << labelToCode(c.container_label) << "," << c.container_id << ","
         << statusToCode(c.status) << "," << c.gross_weight << ","
         << c.customs_declaration_no << "\n";
    count++;
  }

  fout.close();
  cout << "[PERSISTENCE] Da tu dong luu tong cong " << count
       << " container ra file '" << filename << "'.\n";
  return true;
}
