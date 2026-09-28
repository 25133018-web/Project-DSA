#include "Structures.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue &ExportQueue) {

  int user_interactions; // Biến: Số lượng thao tác
  cout << "NHAP SO LUONG THAO TAC BAN CAN: ";
  cin >> user_interactions;

  while (user_interactions--) {
    string user_command;
    cin >> user_command;

    // Thực thi lệnh xuất khẩu
    if (user_command == "EXPORT") {
      string commandded_id;
      cin >> commandded_id;

      ExportContainer(commandded_id, VesselMap, ExportQueue);
    }

    // Cẩu container(s) trong danh sách xuất khẩu có thứ tự lên tàu
    if (user_command == "GET_EXPORT_QUEUE") {
      PrintOrderedExportedContainerQueue(ExportQueue);
    }
  }
}