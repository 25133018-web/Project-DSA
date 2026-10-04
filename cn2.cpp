#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>
#include <iterator> 
using namespace std;

enum class Status_Container{
	pre_gate = 1,// cho vao cong
	in_yard = 2,//da duoc add vao trong bai
	released = 3// da giai phong khoi bai
};

enum class Label_Container{
	DANGER = 1,// hang nguy hiem
	RF = 2,// hang dong lanh
	GP = 3 // hang thuong
};

struct Container{
	Label_Container container_label; // nhan container
	string container_id; // id container
    Status_Container status; // trang thai container
    double gross_weight; // khoi luong container
    string customs_declaration_no; // mÃ£ to khai
};

string statusToString(Status_Container status) {
    switch (status) {
        case Status_Container::pre_gate: return "Pre Gate";
        case Status_Container::in_yard:  return "In Yard";
        case Status_Container::released: return "Released";
        default: return "Unknown";
    }
}

string labelToString(Label_Container label) {
    switch (label) {
        case Label_Container::DANGER: return "Hang nguy hiem";
        case Label_Container::RF:     return "Hang dong lanh";
        case Label_Container::GP:     return "Hang thuong";
        default: return "Unknown";
    }
}

void displayContainer(const Container& c) {
    cout << "\n----------------------------------\n";
    cout << "ID Container : " << c.container_id << "\n";
    cout << "Loai Nhan    : " << labelToString(c.container_label) << "\n";
    cout << "Trang Thai   : " << statusToString(c.status) << "\n";
    cout << "Khoi Luong   : " << c.gross_weight << " tan\n";
    cout << "Ma To Khai   : " << c.customs_declaration_no << "\n";
    cout << "----------------------------------\n";
}

struct Vessel
{ 
    list<Container>ContainerList; 
};

//gom nhom bang dsu
class DSU{
	private: 
		unordered_map<string, string> parent;
		unordered_map<string, int> rankMap;
		unordered_map<string, vector<string>> groupMembers;
	public:
		//thao tac them container vao DSU neu chua co
		void addContainer(const string& containerID){// truyen vao id container
			if (parent.find(containerID) == parent.end()){
				parent[containerID] = containerID;
				rankMap[containerID] = 0;
				groupMembers[containerID] = {containerID};
			}
		}
		bool contains(const string& containerID) const {
			return parent.find(containerID) != parent.end();
		}
		//thao tac find 
		string find(const string& containerID){
			// tu dong khoi tạo neu chua ton tai
			if(parent.find(containerID)== parent.end()){
				addContainer(containerID);
				return containerID;
			}
			//pass1: find root
			string root = containerID;
			while (parent[root] != root){
				root = parent[root];
			}
			//pass 2: nén đường đi
			string current = containerID;

			while (current != root){
				string next = parent[current];
				parent[current] = root;
				current = next;
			}
			return root;
		}
		
		// thao tac union: gop 2 container vao chung mot nhom by rank
		bool unite(const string& containerA, const string& containerB) {
        	string rootA = find(containerA);
        	string rootB = find(containerB);
        	if (rootA == rootB) return false; // đã cùng nhóm rồi

        	// Xac dinh newRoot (gốc mới) va childRoot (gốc cũ bị gộp vào)
        	string newRoot = rootA;
        	string childRoot = rootB;

        	if (rankMap[rootA] < rankMap[rootB]) {
            	newRoot = rootB;
            	childRoot = rootA;
        	} 
			else if (rankMap[rootA] == rankMap[rootB]) {
           	 	rankMap[rootA]++;
        	}

        	parent[childRoot] = newRoot;
        	// Chuyen toan bo thanh vien tu childRoot sang newRoot
        	auto& listNew = groupMembers[newRoot];
        	auto& listChild = groupMembers[childRoot];
        	listNew.insert(listNew.end(), make_move_iterator(listChild.begin()), make_move_iterator(listChild.end()));
        	groupMembers.erase(childRoot);

        	return true;
    	}

		// kiem tra 2 container co cung nhom khong
		bool connected(const string& a, const string& b){
			if (!contains(a) || !contains(b)) return false;
			return find(a) == find(b);
		}

		// Truy van O(1) danh sach container cung nhom
    	vector<string> getLinkedContainers(const string& id) {
        	if (!contains(id)) return {};
        	string root = find(id);
        	return groupMembers[root];
    	}
}; 

// ===================== GOM NHOM CONTAINER CO CUNG MTK =====================
// duyet vesselMap va gom nhom

void groupContainersByDeclaration(unordered_map<string, Vessel>& VesselMap,
                                  DSU& dsu,
                                  unordered_map<string, Container*>& containerLookup){
	unordered_map<string, string> declarationToContainer;
	
	// duyet qua tung tau (vessel) trong VesselMap
	for (auto& [vesselID, vessel] : VesselMap){
		//duyet tung container trong listcontainer cua vessel
		for(auto& c : vessel.ContainerList){
			// kiem tra trung ID container
			if (containerLookup.find(c.container_id) != containerLookup.end()){
				cout << "[LOI] Container " << c.container_id
				     << " (tau " << vesselID << ") bi trung ID, bo qua.\n";
				continue;
			}
			
			containerLookup[c.container_id] = &c;// lấy địa chỉ ô nhớ (&c) của container trong VesselMap gán vào map tra cứu.
            dsu.addContainer(c.container_id);

			// ma to khai rong: khong gop vao nhom nao
			if (c.customs_declaration_no.empty()){
				cout << "[CANH BAO] Container " << c.container_id
				     << " khong co ma to khai, khong gom nhom.\n";
				continue;
			}
				//Kiem tra MTK nay da co trong dsu chua
			auto it = declarationToContainer.find(c.customs_declaration_no);
			if (it != declarationToContainer.end()){
				dsu.unite(it->second, c.container_id);
			}
			else{
				declarationToContainer[c.customs_declaration_no] = c.container_id;
			}
		}
	}
	
}

void XuatThongTinCungMaToKhai(DSU& dsu,
                              const unordered_map<string, Container*>& containerLookup,
                              const string& target_id){
    if (containerLookup.find(target_id) == containerLookup.end()){
		cout << "Khong tim thay container " << target_id << "!\n";
		return;
	}
	vector<string> linkedGroup = dsu.getLinkedContainers(target_id);

    cout << "CAC CONTAINER CHUNG TO KHAI VOI " << target_id << ":" << endl;
    cout << "==========================================" << endl;

    for (const string& id : linkedGroup){
		auto it = containerLookup.find(id);
		if (it != containerLookup.end()){
			displayContainer(*(it->second));
		}
	}
	if (linkedGroup.size() <= 1){
		cout << "(Khong co container nao khac lien ket voi container nay)\n";
	}
	
}

// ===================== GOP NHOM CONTAINER KHI CHUNG THUỘC 1  LÔ HÀNG LỚN =====================
void gopNhomContainer(DSU& dsu, unordered_map<string, Container*>& containerLookup){
	string idA, idB;
	string idA, idB;
	cout << "Nhap ID container thuoc nhom thu nhat: ";
	cin >> idA;
	cout << "Nhap ID container thuoc nhom thu hai : ";
	cin >> idB;

	if (containerLookup.find(idA) == containerLookup.end()){
		cout << "Khong tim thay container " << idA << "!\n";
		return;
	}
	if (containerLookup.find(idB) == containerLookup.end()){
		cout << "Khong tim thay container " << idB << "!\n";
		return;
	}

	if (dsu.unite(idA, idB)) {
        // Lay ma to khai đại diện (uu tien idA, neu rong lay idB)
        string mainDeclaration = containerLookup[idA]->customs_declaration_no;
        if (mainDeclaration.empty()) {
            mainDeclaration = containerLookup[idB]->customs_declaration_no;
        }

        // Dong bo ma to khai cho tat ca thanh vien trong nhom moi
        vector<string> mergedGroup = dsu.getLinkedContainers(idA);
        for (const string& id : mergedGroup) {
            containerLookup[id]->customs_declaration_no = mainDeclaration;
        }
        cout << "Da gop nhom cua " << idA << " va " << idB << " thanh mot nhom.\n";
        cout << "Nhom moi hien co " << mergedGroup.size() << " container lien thong.\n";
    } 
	else {
        cout << idA << " va " << idB << " da thuoc cung mot nhom, khong can gop.\n";
    }
}

int main (){
	unordered_map < string, Vessel > VesselMap;
	VesselMap["MSCU"].ContainerList.push_back({Label_Container::RF,"MSCU1234566", Status_Container::in_yard, 300.45, "TK_1001"});
    VesselMap["MAEU"].ContainerList.push_back({Label_Container::GP,"MAEU9876542", Status_Container::in_yard, 200.50, "TK_1002"});
    VesselMap["TGHU"].ContainerList.push_back({Label_Container::GP,"TGHU4567896", Status_Container::in_yard, 400.00, "TK_1001"});
    VesselMap["CNOU"].ContainerList.push_back({Label_Container::DANGER,"CNOU3216549", Status_Container::in_yard, 445.30, "TK_1003"});
    VesselMap["BSIU"].ContainerList.push_back({Label_Container::DANGER,"BSIU8529636", Status_Container::in_yard, 445.30, "TK_1001"});
	// du lieu thu loi: ma to khai rong va trung ID
	VesselMap["EMPT"].ContainerList.push_back({Label_Container::GP,     "EMPT0000001", Status_Container::pre_gate, 100.00, ""});
	DSU dsu;
	unordered_map<string, Container*> containerLookup;
    groupContainersByDeclaration(VesselMap, dsu, containerLookup);
	
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
	return 0;
}


