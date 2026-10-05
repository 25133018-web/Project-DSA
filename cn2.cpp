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
//Thao tác 1:Của Lê Anh Hào
//================================Quản lý và gộp nhóm container===========================//
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



//Thao tac 2: Cua Do Nguyet Hanh
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




	return 0;
}


