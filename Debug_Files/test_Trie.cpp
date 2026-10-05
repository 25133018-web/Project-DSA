#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/Trie.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

    // --- BUOC 1: CHUAN BI DU LIEU MAU DUNG FORMAT PROJECT ---
    Container c1 = {Label_Container::GP, "MAEU1234567", Status_Container::in_yard, 24.5, "TK001"};
    Container c2 = {Label_Container::DANGER, "MAEU9999999", Status_Container::in_yard, 30.0, "TK002"};
    Container c3 = {Label_Container::RF, "CBHU1234567", Status_Container::released, 18.2, "TK003"};

    // --- BUOC 2: NAP DU LIEU VAO TRIE ---
    trie.insert(&c1);
    trie.insert(&c2);
    trie.insert(&c3);

    // --- BUOC 3: CAC TEST CASE KIEM THU ---
    // Test 1: Tim goi y tien to hop le
    vector<Container *> res1 = trie.searchbyprefix("MAEU");
    CHECK("Test 1: Tim thay du 2 container voi tien to 'MAEU'", res1.size() == 2);

    // Test 2: Goi y khong phan biet hoa/thuong
    vector<Container *> res2 = trie.searchbyprefix("cbhu");
    CHECK("Test 2: Tim chuoi thuong 'cbhu' tu dong chuyen thanh 'CBHU'",
          res2.size() == 1 && res2[0]->container_id == "CBHU1234567");

    // Test 3: Tien to ngan (< 2 ky tu)
    vector<Container *> res3 = trie.searchbyprefix("M");
    CHECK("Test 3: Tien to ngan (< 2 ky tu) tra ve danh sach rong", res3.empty());

    // Test 4: Tien to khong ton tai
    vector<Container *> res4 = trie.searchbyprefix("TEXU");
    CHECK("Test 4: Tien to khong ton tai 'TEXU' tra ve danh sach rong", res4.empty());

    // Test 5: Tien to chua ky tu dac biet
    vector<Container *> res5 = trie.searchbyprefix("MA@#");
    CHECK("Test 5: Tien to chua ky tu dac biet tra ve danh sach rong", res5.empty());

    // Test 6: Xoa container khoi Trie
    trie.remove("MAEU1234567");
    vector<Container *> res6 = trie.searchbyprefix("MAEU");
    CHECK("Test 6: Sau khi remove 'MAEU1234567', tien to 'MAEU' con 1 container",
          res6.size() == 1 && res6[0]->container_id == "MAEU9999999");

    // Test 7: showInfo xu ly an toan voi nullptr va container hop le
    cout << "\n--- Kiem tra showInfo() ---" << endl;
    cout << "Goi showInfo voi container hop le:" << endl;
    trie.showInfo(&c2);
    cout << "Goi showInfo voi con tro nullptr (khong duoc crash):" << endl;
    trie.showInfo(nullptr);
    CHECK("Test 7: showInfo xu ly an toan voi con tro nullptr", true);

    // --- TONG KET ---
    cout << "\n======================================================" << endl;
    cout << "   KET QUA: " << passed_tests << "/" << total_tests << " TEST CASES PASSED!" << endl;
    if (passed_tests == total_tests) {
        cout << "   [PASS] TAT CA CAC TEST CHO TRIE DEU DAT!" << endl;
    } else {
        cout << "   [FAIL] CO TEST THAT BAI!" << endl;
    }
    cout << "======================================================" << endl;

    return passed_tests == total_tests ? 0 : 1;
}
