//MAIN NAY CHO NHOM TRUONG, KHONG DUOC THAO TAC XOA, SUA, THEM VAO DAY NHE
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

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
    string customs_declaration_no; // mã to khai
};

/*
// ham lay ma so nguyen giong getStatus trong java
// neu muon dat ma cho trang thai thi de lai khong thi xoa di
inline int getStatus(Status_Container status){
	return static_cast<int>(status);
}
*/

//Ham de chuyen doi Enum sang chuoi mo ta khi output
string statusToString(Status_Container status){
	switch(status){
		case Status_Container::pre_gate: return "Pre Gate";
		case Status_Container::in_yard: return "In Yard";
		case Status_Container::released: return "Realeased";
		default: return "Unknown";
}

string labelToString(Label_Container label){
	switch(status){
		case Label_Container::DANGER: return "Hang nguy hiem";
		case Label_Container::RF: return "Hang dong lanh";
		case Label_Container::GP: return "Hang thuong";
		default: return "Unknown";
}
Container inputContainer(){
	Container c;
	cout <<"----Nhap vao thong tin Container----\n";
	cout <<"Nhap ID Container: ";
	cin>>c.container_id;
	
	int labelChoice;
	do{
		cout<<"Chon loai nhan:\n";
		cout <<"1. DANGER (Hang nguy hiem)\n";
		cout <<"2. RF (Hang dong lanh)\n";
		cout <<"3. GP (Hang thuong)\n";
		cout <<"Lua chon:\n";
		cin >> labelChoice;
		if (labelChoice < 1 || labelChoice > 3) {
            cout << "=> Lua chon khong hop le! Vui long chon lai!\n";
        }
    } while (labelChoice < 1 || labelChoice > 3);
    
    c.container_label = static_cast<Label_Container>(labelChoice);
    
    int statusChoice;
    do {
        cout << "Chon Trang Thai:\n";
        cout << "  1. Pre Gate (Cho vao cong)\n";
        cout << "  2. In Yard (Trong bai)\n";
        cout << "  3. Released (Da gia phong)\n";
        cout << "Lua chon cua ban (1-3): ";
        cin >> statusChoice;
        if (statusChoice < 1 || statusChoice > 3) {
            cout << "=> Lua chon khong hop le! Vui long chon lai!\n";
        }
    } while (statusChoice < 1 || statusChoice > 3);
    c.status = static_cast<Status_Container>(statusChoice);

    cout << "Nhap khoi luong (tan): ";
    cin >> c.gross_weight;

    cout << "Nhap ma to khai quan: ";
    cin >> c.customs_declaration_no;

    return c;	
}
void displayContainer(const Container& c){
	cout << "----------------------------------\n";
	cout << "ID Container : " << c.container_id << "\n";
    cout << "Loai Nhan    : " << labelToString(c.container_label) << "\n";
    cout << "Trang Thai   : " << statusToString(c.status) << "\n";
    cout << "Khoi Luong   : " << c.gross_weight << " tan\n";
    cout << "Ma To Khai   : " << c.customs_declaration_no << "\n";
}
// danh sach dang cho toi cong, add vao hang doi khi no dung va khong trung lap
// neu chua thi add vao, con co roi thi? 
//roi chuc nang tim kiem mc1 la sao?
int main (){
	unnordered_map <string, Container> containerMap;
	
	//gioi han suc chua cua cang?
	
	int n;
	cout << "Nhap vao so container muon nhap: ";
	
}
	





