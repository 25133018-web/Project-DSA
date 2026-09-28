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
    list<Container>ContainerList; 
};

//gom nhom bang dsu
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


