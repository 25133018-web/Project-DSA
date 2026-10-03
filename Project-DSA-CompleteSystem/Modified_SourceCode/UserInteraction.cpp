#include "YardSystem.h"

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue &ExportQueue) {

  cout << "BAT DAU NHAP LENH:\n*Nhap DONE de ket thuc.\n";

  while (true) {
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

    if (user_command == "DONE") {
      break;
    }
  }
}