#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>
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

//gom nhom bang dsu //đoạn Hào mới sửa từ đây
class DSU{
	private: 
		unordered_map<string, string> parent;
		unordered_map<string, int> rankMap;
	public:
		//thao tac them container vao DSU neu chua co
		void addContainer(const string& containerID){// truyen vao id container
			if (parent.find(containerID) == parent.end()){
				parent[containerID] = containerID;
				rankMap[containerID] = 0;
			}
		}
		
//thao tac find 
		string find(const string& containerID){
			// tu dong khoi tạo neu chua ton tai
			if(parent.find(containerID)== parent.end()){
				addContainer(containerID);
				return containerID;
			}
			//find root
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

        	if (rankMap[rootA] < rankMap[rootB]) {
            	parent[rootA] = rootB;
        	} 	
			else if (rankMap[rootA] > rankMap[rootB]) {
            parent[rootB] = rootA;
        	} 
			else {
            	parent[rootB] = rootA;
            	rankMap[rootA]++;
        	}
        	return true;
    	}

		// Lấy tất cả container thuộc chung nhóm với container target_id
    	vector<string> getLinkedContainers(const string& id) {
        	if (parent.find(id) == parent.end()) return {};

        	string targetRoot = find(id);
        	vector<string> result;

        	// Duyệt qua tất cả các container đã lưu và tìm những nút có cùng gốc
        	for (const auto& [containerID, _] : parent) {
           		if (find(containerID) == targetRoot) {
                result.push_back(containerID);
            }
        }
        return result;
    }
}; //tới đây
// duyet vesselMap va gom nhom

void groupContainersByDeclaration (const unordered_map<string, Vessel>& VesselMap, DSU& dsu, unordered_map<string, Container>& containerLookup ){
	unordered_map<string, string> declarationToContainer;
	
	// duyet qua tung tau (vessel) trong VesselMap
	for (const auto& [vesselID, vessel] : VesselMap){
		//duyet tung container trong listcontainer cua vessel
		for( const auto& Container : vessel.ContainerList){
			// khoi tao container vao dsu
			containerLookup[Container.container_id] = Container;
			dsu.addContainer(Container.container_id);
				//Kiem tra MTK nay da co trong dsu chua
				if(declarationToContainer.find(Container.customs_declaration_no) != declarationToContainer.end()){
					//neu da co container co cung mtk truoc do thi tien hanh gop nhom
					string previousContainerID = declarationToContainer[Container.customs_declaration_no];
				 	dsu.unite(previousContainerID, Container.container_id);
				}
				else{
					//lan dau thay ma to khai nay thi lay container do lam dai dien
					declarationToContainer[Container.customs_declaration_no] = Container.container_id;
				}
		}
	}
	
}

void XuatThongTinCungMaToKhai(const unordered_map<string, Vessel>& VesselMap, const string& target_id){
	DSU dsu;
	unordered_map<string, Container> containerLookup;
    groupContainersByDeclaration(VesselMap, dsu, containerLookup);

    vector<string> linkedGroup = dsu.getLinkedContainers(target_id);

    cout << "CAC CONTAINER CHUNG TO KHAI VOI " << target_id << ":" << endl;
    cout << "==========================================" << endl;

    if (linkedGroup.empty()) {
        cout << "Khong tim thay container nao!" << endl;
    } else {
        for (const string& id : linkedGroup) {
            if (containerLookup.find(id) != containerLookup.end()) {
                displayContainer(containerLookup[id]);
            }
        }
    }
	
}

int main (){
	unordered_map < string, Vessel > VesselMap;
	VesselMap["MSCU"].ContainerList.push_back({Label_Container::RF,"MSCU1234566", Status_Container::in_yard, 300.45, "TK_1001"});
    VesselMap["MAEU"].ContainerList.push_back({Label_Container::GP,"MAEU9876542", Status_Container::in_yard, 200.50, "TK_1002"});
    VesselMap["TGHU"].ContainerList.push_back({Label_Container::GP,"TGHU4567896", Status_Container::in_yard, 400.00, "TK_1001"});
    VesselMap["CNOU"].ContainerList.push_back({Label_Container::DANGER,"CNOU3216549", Status_Container::in_yard, 445.30, "TK_1003"});
    VesselMap["BSIU"].ContainerList.push_back({Label_Container::DANGER,"BSIU8529636", Status_Container::in_yard, 445.30, "TK_1001"});
	string targetID;
	cout <<"Nhap ID container muon tra cuu: ";
	cin >>targetID;
	XuatThongTinCungMaToKhai(VesselMap, targetID);
    return 0;
}


