#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Trie.h"
using namespace std;

enum class Label_Container { GP, DANGER, REEFER };
enum class Status_Container { in_yard, exported };

string labelToString(Label_Container l) {
    switch (l) {
        case Label_Container::GP: return "General Purpose (GP)";
        case Label_Container::DANGER: return "Danger";
        case Label_Container::REEFER: return "Reefer";
    }
    return "Unknown";
}

string statusToString(Status_Container s) {
    switch (s) {
        case Status_Container::in_yard: return "In Yard";
        case Status_Container::exported: return "Exported";
    }
    return "Unknown";
}

struct Container {
    string container_id;
    Label_Container container_label;
    Status_Container status;
    double gross_weight;
    string customs_declaration_no;
};

// Import file Trie.h của bạn vào đây
#include "Trie.h"

int total_tests = 0;
int passed_tests = 0;

void CHECK(const string &ten_test, bool dieu_kien) {
    total_tests++;
    if (dieu_kien) {
        passed_tests++;
        cout << "  [PASS] " << ten_test << endl;
    } else {
        cout << "  [FAIL] " << ten_test << " ---> LOI (Kiem tra lai logic!)" << endl;
    }
}

int main() {
    cout << "======================================================" << endl;
    cout << "   BAT DAU KIEM THU TU DONG: ContainerTrie (Trie.h)  " << endl;
    cout << "======================================================" << endl << endl;

    ContainerTrie trie;

    // --- BƯỚC 1: CHUẨN BỊ DỮ LIỆU MAU ---
    Container c1 = {"MAEU1234567", Label_Container::GP, Status_Container::in_yard, 24.5, "TK001"};
    Container c2 = {"MAEU9999999", Label_Container::DANGER, Status_Container::in_yard, 30.0, "TK002"};
    Container c3 = {"CBHU1234567", Label_Container::REEFER, Status_Container::exported, 18.2, "TK003"};

    // --- BƯỚC 2: NẠP DỮ LIỆU VÀO TRIE ---
    trie.insert(&c1);
    trie.insert(&c2);
    trie.insert(&c3);


    // Test 1: goi y tien to
    vector<Container*> res1 = trie.searchbyprefix("MAEU");
    CHECK("Test 1: Tim thay du 2 container voi tien to 'MAEU'", res1.size() == 2);

    // Test 2: in hoa in thuong 
    vector<Container*> res2 = trie.searchbyprefix("cbhu");
    CHECK("Test 2: Tim chuoi thuong 'cbhu' tu dong chuyen thanh 'CBHU'", res2.size() == 1 && res2[0]->container_id == "CBHU1234567");

    // Test 3: tien to ngan hon 2 ki tu 
    vector<Container*> res3 = trie.searchbyprefix("M");
    CHECK("Test 3: Tien to ngan (< 2 ky tu) tra ve danh sach rong", res3.empty() == true);

    // Test 4 tien to khong ton tai
    vector<Container*> res4 = trie.searchbyprefix("TEXU");
    CHECK("Test 4: Tien to khong ton tai 'TEXU' tra ve danh sach rong", res4.empty() == true);

    // Test 5: tien to khong chua ki tu dac biet
    vector<Container*> res5 = trie.searchbyprefix("MA@#");
    CHECK("Test 5: Tien to chua ky tu dac biet tra ve danh sach rong", res5.empty() == true);


    cout << "   KET QUA KIEM THU CÁ NHAN: " << passed_tests << "/" << total_tests << " TEST CASES PASSED!" << endl;
    if (passed_tests == total_tests) {
        cout << "  [PASS] << endl;
    } else {
        cout << "[FAIL]" << endl;
    }
    return 0;
}