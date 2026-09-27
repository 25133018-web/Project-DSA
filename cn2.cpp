#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <list>
using namespace std;
struct Container{
	string container_label; // nhan container
	string container_id; // id container
    string status; // trang thai container
    double gross_weight; // khoi luong container
    string customs_declaration_no; // ma to khai
};

struct Vessel
{ 
    list<Container>ContainerList; //
};

//gom nhom bang dsu
class DSU{
	private: 
		unordered_map<string, string> parent;
		unordered_map<string, vector<string>> groups;
	public:
		//thao tac them container vao DSU neu chua co
		void addContainer(const string& id){// truyen vao id container
			if (parent.find(id) == parent.end()){
				parent[id] = id;
				groups[id] = {id};
			}
		}
		
		//thao tac find 
		string find(string id){
			if(parent[id] == id) return id;
			return parent[id] = find(parent[id]);
		}
		
		// thao tac union: gop 2 container vao chung mot nhom
		void unite(string id1, string id2){
			// tu dong them vao tranh case empty
			addContainer(id1);
        	addContainer(id2);
        	
			string root1 = find(id1);
			string root2 = find(id2);
			
			if(root1 != root2){
				// gop 2 container lay root1 lam dai dien
				parent[root2] = root1;	
				// chuyen danh sach container o root2 sang root1
				groups[root1].insert(groups[root1].end(), groups[root2].begin(), groups[root2].end());
				groups.erase(root2); 
			}
		}
		
		// lay tat ca container thuoc chung nhom voi container id
		vector<string> getLinkedContainers(string id){
			if(parent.find(id)== parent.end()) return {};
			string root = find(id);
			return groups[root];
		}
};

// duyet vesselMap va gom nhom

void groupContainersByDeclaration (const unordered_map<string, Vessel>& VesselMap, DSU& dsu ){
	unordered_map<string, string> declarationToContainer;
	
	// duyet qua tung tau (vessel) trong VesselMap
	for (const auto& [vesselID, vessel] : VesselMap){
		//duyet tung container trong listcontainer cua vessel
		for( const auto& Container : vessel.ContainerList){
			// khoi tao container vao dsu
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
			
int main (){
	unordered_map < string, Vessel > VesselMap;
	
	VesselMap["MEU"].ContainerList.push_back({"Hang dong lanh","CONT_01", "O cang", 300.45, "TK_1001"});
    VesselMap["MEU"].ContainerList.push_back({"Hang thuong","CONT_02","O cang", 200.50,"TK_1002"});
    VesselMap["ONE"].ContainerList.push_back({"Hang thuong","CONT_03", "O cang",400,"TK_1001"});
    VesselMap["ONE"].ContainerList.push_back({"Hang nguy hiem","CONT_04","O cang",445.30, "TK_1003"});
    DSU dsu;
    groupContainersByDeclaration(VesselMap, dsu);

    vector<string> linkedGroup = dsu.getLinkedContainers("CONT_01");

    cout << "Cac container lien quan chung to khai can xu ly chung:" << endl;
    for (const string& id : linkedGroup) {
        cout << " -> " << id << endl;
    }

    return 0;
}


