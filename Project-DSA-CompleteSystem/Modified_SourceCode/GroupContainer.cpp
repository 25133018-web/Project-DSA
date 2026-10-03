#include "DSU.h"

void displayContainer(const Container &c) {
  cout << "\n----------------------------------\n";
  cout << "ID Container : " << c.container_id << "\n";
  cout << "Loai Nhan    : " << labelToString(c.container_label) << "\n";
  cout << "Trang Thai   : " << statusToString(c.status) << "\n";
  cout << "Khoi Luong   : " << c.gross_weight << " tan\n";
  cout << "Ma To Khai   : " << c.customs_declaration_no << "\n";
  cout << "----------------------------------\n";
}

// ===================== GOM NHOM CONTAINER CO CUNG MTK =====================
// duyet vesselMap va gom nhom

void groupContainersByDeclaration(
    const unordered_map<string, Vessel> &VesselMap, DSU &dsu,
    unordered_map<string, Container> &containerLookup) {
  unordered_map<string, string> declarationToContainer;

  // duyet qua tung tau (vessel) trong VesselMap
  for (const auto &[vesselID, vessel] : VesselMap) {
    // duyet tung container trong listcontainer cua vessel
    for (const auto &Container : vessel.ContainerList) {
      // kiem tra trung ID container
      if (containerLookup.find(Container.container_id) !=
          containerLookup.end()) {
        cout << "[LOI] Container " << Container.container_id << " (tau "
             << vesselID << ") bi trung ID, bo qua.\n";
        continue;
      }
      // khoi tao container vao dsu
      containerLookup[Container.container_id] = Container;
      dsu.addContainer(Container.container_id);
      // ma to khai rong: khong gop vao nhom nao
      if (Container.customs_declaration_no.empty()) {
        cout << "[CANH BAO] Container " << Container.container_id
             << " khong co ma to khai, khong gom nhom.\n";
        continue;
      }
      // Kiem tra MTK nay da co trong dsu chua
      auto it = declarationToContainer.find(Container.customs_declaration_no);
      if (it != declarationToContainer.end()) {
        dsu.unite(it->second, Container.container_id);
      } else {
        declarationToContainer[Container.customs_declaration_no] =
            Container.container_id;
      }
    }
  }
}

void XuatThongTinCungMaToKhai(
    DSU &dsu, const unordered_map<string, Container> &containerLookup,
    const string &target_id) {
  if (containerLookup.find(target_id) == containerLookup.end()) {
    cout << "Khong tim thay container " << target_id << "!\n";
    return;
  }
  vector<string> linkedGroup = dsu.getLinkedContainers(target_id);

  cout << "CAC CONTAINER CHUNG TO KHAI VOI " << target_id << ":" << endl;
  cout << "==========================================" << endl;

  for (const string &id : linkedGroup) {
    auto it = containerLookup.find(id);
    if (it != containerLookup.end()) {
      displayContainer(it->second);
    }
  }
  if (linkedGroup.size() == 1) {
    cout << "(Khong co container nao khac lien ket voi container nay)\n";
  }
}

// ===================== GOP NHOM CONTAINER KHI CHUNG THUỘC 1  LÔ HÀNG LỚN
// =====================
void gopNhomContainer(DSU &dsu,
                      const unordered_map<string, Container> &containerLookup) {
  string idA, idB;
  cout << "Nhap ID container thuoc nhom thu nhat: ";
  cin >> idA;
  cout << "Nhap ID container thuoc nhom thu hai : ";
  cin >> idB;

  if (containerLookup.find(idA) == containerLookup.end()) {
    cout << "Khong tim thay container " << idA << "!\n";
    return;
  }
  if (containerLookup.find(idB) == containerLookup.end()) {
    cout << "Khong tim thay container " << idB << "!\n";
    return;
  }

  if (dsu.unite(idA, idB)) {
    cout << "Da gop nhom cua " << idA << " va " << idB << " thanh mot nhom.\n";
    cout << "Nhom moi hien co " << dsu.getLinkedContainers(idA).size()
         << " container.\n";
  } else {
    cout << idA << " va " << idB << " da thuoc cung mot nhom, khong can gop.\n";
  }
}
