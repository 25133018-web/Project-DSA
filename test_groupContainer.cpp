#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cassert>
#include "YardSystem.h"

using namespace std;

// Test1: Kiem tra gom nhom cac container co cung MTK
void test_GroupContainers_SameDeclaration_ShouldUnite() {
    unordered_map<string, Vessel> vesselMap;
    DSU dsu;
    unordered_map<string, Container*> containerLookup; 

    // Them container vao DSU truoc khi gom nhom
    dsu.addContainer("MSCU1234566");
    dsu.addContainer("TGHU4567896");

    vesselMap["MSCU"].ContainerList.push_back({Label_Container::RF, "MSCU1234566", Status_Container::in_yard, 300.45, "TK_1001"});
    vesselMap["TGHU"].ContainerList.push_back({Label_Container::GP, "TGHU4567896", Status_Container::in_yard, 400.00, "TK_1001"});

    groupContainersByDeclaration(vesselMap, dsu, containerLookup);

    string root1 = dsu.find("MSCU1234566");
    string root2 = dsu.find("TGHU4567896");

    // Kiem tra điukien:
    // 1. ca 2 container deu nam trong lookup
    // 2. Goc DSU cua 2 container phai GIONG NHAU va KHONG RONG
    if (containerLookup.size() == 2 && !root1.empty() && root1 == root2) {
        cout << "PASSED Test 1: Gom nhom cung ma to khai thanh cong!\n";
    } 
    else {
        cout << "FAILED Test 1: Gom nhom cung ma to khai THAT BAI!\n";
        cout << "   [DEBUG] Lookup size: " << containerLookup.size() << " (Kỳ vọng: 2)\n";
        cout << "   [DEBUG] Root MSCU1234566: '" << root1 << "'\n";
        cout << "   [DEBUG] Root TGHU4567896: '" << root2 << "'\n";
    }
}

// Test2: Container khac ma to khai khong duoc gom nhom
void test_GroupContainers_DifferentDeclaration_ShouldNotUnite() {
    unordered_map<string, Vessel> vesselMap;
    DSU dsu;
    unordered_map<string, Container*> containerLookup;

    dsu.addContainer("MSCU1234566");
    dsu.addContainer("MAEU9876542");

    vesselMap["MSCU"].ContainerList.push_back({Label_Container::RF, "MSCU1234566", Status_Container::in_yard, 300.45, "TK_1001"});
    vesselMap["MAEU"].ContainerList.push_back({Label_Container::GP, "MAEU9876542", Status_Container::in_yard, 200.50, "TK_1002"});
    
    groupContainersByDeclaration(vesselMap, dsu, containerLookup);

    if (dsu.find("MSCU1234566") != dsu.find("MAEU9876542")) {
        cout << "PASSED Test 2: Khac ma to khai khong bi gop nhom!\n";
    } 
    else {
        cout << "FAILED Test 2: Khac ma to khai bi gop nhom sai!\n";
    }
}

// Test3: Container co ma to khai rong khong duoc gom nhom
void test_GroupContainers_EmptyDeclaration_ShouldNotUnite() {
    unordered_map<string, Vessel> vesselMap;
    DSU dsu;
    unordered_map<string, Container*> containerLookup;

    dsu.addContainer("MSCU1234566");
    dsu.addContainer("MAEU9876542");

    vesselMap["MSCU"].ContainerList.push_back({Label_Container::RF, "MSCU1234566", Status_Container::in_yard, 300.45, ""});
    vesselMap["MAEU"].ContainerList.push_back({Label_Container::GP, "MAEU9876542", Status_Container::in_yard, 200.50, ""});

    cout << "--- Log kiem tra canh bao (Neu co) ---\n";
    groupContainersByDeclaration(vesselMap, dsu, containerLookup);
    cout << "-------------------------------------\n";

    if (dsu.find("MSCU1234566") != dsu.find("MAEU9876542")) {
        cout << "PASSED Test 3: Ma to khai rong khong bi gop nhom!\n";
    } else {
        cout << "FAILED Test 3: Ma to khai rong bi gop nhom sai!\n";
    }
}

// Test4: Tim kiem container khong ton tai
void test_XuatThongTinCungMaToKhai_NotFound() {
    unordered_map<string, Vessel> vesselMap;
    DSU dsu;
    unordered_map<string, Container*> containerLookup;
    cout << "--- Log ket qua tim kiem ID khong ton tai ---\n";
    XuatThongTinCungMaToKhai(dsu, containerLookup, "C999");
    cout << "--------------------------------------------\n";
    cout << "PASSED Test 4: Tim container khong ton tai chay binh thuong!\n";
}

// Test5: gop nhom thu cong va dong bo ma to khai
void test_GopNhomContainer_ValidAndSync() {
    unordered_map<string, Vessel> vesselMap;
    DSU dsu;
    unordered_map<string, Container*> containerLookup;

    dsu.addContainer("MSCU1234566");
    dsu.addContainer("MAEU9876542");

    vesselMap["MSCU"].ContainerList.push_back({Label_Container::RF, "MSCU1234566", Status_Container::in_yard, 300.45, "TK_1001"});
    vesselMap["MAEU"].ContainerList.push_back({Label_Container::GP, "MAEU9876542", Status_Container::in_yard, 200.50, "TK_1002"});
    
    groupContainersByDeclaration(vesselMap, dsu, containerLookup);

    stringstream fakeInput;
    fakeInput << "MSCU1234566\nMAEU9876542\n";
    streambuf* origCin = cin.rdbuf(fakeInput.rdbuf());

    gopNhomContainer(dsu, containerLookup);

    cin.rdbuf(origCin);

    if (dsu.find("MSCU1234566") == dsu.find("MAEU9876542") && 
        containerLookup["MAEU9876542"]->customs_declaration_no == "TK_1001") {
        cout << "PASSED Test 5: Gop nhom thu cong & Dong bo ma to khai thanh cong!\n";
    } else {
        cout << "FAILED Test 5: Gop nhom thu cong THAT BAI!\n";
    }
}

// =================================================================
// Ham main chay toan bo cac test
// =================================================================
int main() {
    cout << "================= BAT DAU KIEM THU =================" << endl;
    
    test_GroupContainers_SameDeclaration_ShouldUnite();
    cout << endl;

    test_GroupContainers_DifferentDeclaration_ShouldNotUnite();
    cout << endl;

    test_GroupContainers_EmptyDeclaration_ShouldNotUnite();
    cout << endl;

    test_XuatThongTinCungMaToKhai_NotFound();
    cout << endl;

    test_GopNhomContainer_ValidAndSync();
    cout << endl;

    cout << "================= HOAN THANH KIEM THU =================" << endl;
    return 0;
}