#include "YardSystem.h"

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue &ExportQueue) {
  //Them cua chuc nang 2
  DSU dsu;
  unordered_map<string, Container> containerLookup;
	groupContainersByDeclaration(VesselMap, dsu, containerLookup);
  
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
    //Chuc nang 2
    if (user_command == "GROUP"){
      int choice = -1;
	    while (choice != 0){
		    cout << "\n===== QUAN LY NHOM CONTAINER THEO TO KHAI =====\n";
		    cout << "1. Tra cuu cac container cung nhom\n";
		    cout << "2. Gop 2 nhom container\n";
		    cout << "0. Thoat\n";
		    cout << "Chon: ";
		    if (!(cin >> choice)){
			    cin.clear();
			    cin.ignore(10000, '\n');
			    choice = -1;
			    cout << "Lua chon khong hop le!\n";
			    continue;
		    }

		    if (choice == 1){
			    string targetID;
			    cout << "Nhap ID container muon tra cuu: ";
			    cin >> targetID;
			    XuatThongTinCungMaToKhai(dsu, containerLookup, targetID);
		    }
		    else if (choice == 2){
			    gopNhomContainer(dsu, containerLookup);
		    }
		    else if (choice != 0){
			    cout << "Lua chon khong hop le!\n";
		    }
	    }
    }

    if (user_command == "DONE") {
      break;
    }
  }
}
