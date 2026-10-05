#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <set>
#include <unordered_map>
#include "../CourseProject/Project-DSA-CompleteSystem/Modified_SourceCode_v2/DSU.h"

using namespace std;

int total_tests = 0; int passed_tests = 0;

void CHECK(const string &ten_test, bool dieu_kien) {
    total_tests++;
    if (dieu_kien) {
        passed_tests++;
        cout << "  [PASS] " << ten_test << endl;
    } else {
        cout << "  [FAIL] " << ten_test << " ---> LOI (Kiem tra lai logic!)" << endl;
    }
}

string makeID(int i) {            // MAEU0000000 ... (11 ky tu giong ID container)
    string s = to_string(i);
    return "MAEU" + string(7 - s.size(), '0') + s;
}
double ms(chrono::steady_clock::time_point a, chrono::steady_clock::time_point b) {
    return chrono::duration<double, milli>(b - a).count();
}

int main() {
    const int N = 15000;

    cout << "=== PHAN 1: TEST CHUC NANG & TRUONG HOP BIEN ===" << endl;
    {
        // Buoc 1 (Chuan bi): 3 container mau A, B, C
        DSU d;
        d.addContainer("A"); d.addContainer("B"); d.addContainer("C");

        // Buoc 2 + 3: chay ham roi kiem tra
        CHECK("Test 1: Moi container ban dau la 1 nhom rieng",
              !d.connected("A","B") && d.getLinkedContainers("A").size() == 1);

        bool r1 = d.unite("A","B");                              // Buoc 2
        CHECK("Test 2: unite(A,B) tra ve true", r1 == true);     // Buoc 3
        CHECK("Test 3: A va B cung nhom", d.connected("A","B"));

        bool r2 = d.unite("B","A");
        CHECK("Test 4: unite lan 2 cung cap tra ve false", r2 == false);
        CHECK("Test 5: C van tach rieng", !d.connected("A","C"));

        d.unite("B","C");
        CHECK("Test 6: Bac cau A-B-C => A noi C", d.connected("A","C"));
        CHECK("Test 7: Nhom co dung 3 thanh vien", d.getLinkedContainers("C").size() == 3);
        CHECK("Test 8: unite(A,A) tra ve false", d.unite("A","A") == false);

        // Truong hop bien
        CHECK("Test 9: ID khong ton tai => connected = false", !d.connected("A","KHONG_CO"));
        CHECK("Test 10: ID khong ton tai => getLinkedContainers rong", d.getLinkedContainers("KHONG_CO").empty());
        d.addContainer("A");
        CHECK("Test 11: addContainer trung khong reset nhom", d.getLinkedContainers("A").size() == 3);
        d.unite("X","Y");
        CHECK("Test 12: unite voi ID chua add van hoat dong", d.connected("X","Y"));
    }

    cout << "\n=== PHAN 2: " << N << " CONTAINER ===" << endl;
    // Buoc 1 (Chuan bi): sinh 15000 ID container
    vector<string> ids(N);
    for (int i = 0; i < N; i++) ids[i] = makeID(i);

    // --- 2.1 Nap 15000 container
    DSU d;
    auto t0 = chrono::steady_clock::now();
    for (auto &s : ids) d.addContainer(s);                       // Buoc 2
    auto t1 = chrono::steady_clock::now();
    bool allSingle = true;                                       // Buoc 3
    for (int i = 0; i < N; i += 1000) if (d.getLinkedContainers(ids[i]).size() != 1) allSingle = false;
    CHECK("Test 13: Nap 15000 container, moi cai la nhom rieng", allSingle);
    cout << "     -> addContainer x" << N << ": " << ms(t0,t1) << " ms" << endl;

    // --- 2.2 Gop chuoi 0-1, 1-2, ... (truong hop xau cho cay)
    t0 = chrono::steady_clock::now();
    int unions = 0;
    for (int i = 0; i + 1 < N; i++) if (d.unite(ids[i], ids[i+1])) unions++;   // Buoc 2
    t1 = chrono::steady_clock::now();
    CHECK("Test 14: Gop chuoi: dung N-1 lan unite thanh cong", unions == N-1); // Buoc 3
    CHECK("Test 15: Dau chuoi va cuoi chuoi cung nhom", d.connected(ids[0], ids[N-1]));
    auto all = d.getLinkedContainers(ids[N/2]);
    CHECK("Test 16: Nhom lon co du 15000 thanh vien", (int)all.size() == N);
    set<string> uniq(all.begin(), all.end());
    CHECK("Test 17: Khong trung lap / khong mat thanh vien", (int)uniq.size() == N);
    cout << "     -> " << unions << " unite (chuoi): " << ms(t0,t1) << " ms" << endl;

    // --- 2.3 find 15000 lan sau khi nen duong di
    t0 = chrono::steady_clock::now();
    bool sameRoot = true;
    string r0 = d.find(ids[0]);
    for (auto &s : ids) if (d.find(s) != r0) { sameRoot = false; break; }
    t1 = chrono::steady_clock::now();
    CHECK("Test 18: Tat ca 15000 container chung 1 goc", sameRoot);
    cout << "     -> find x" << N << ": " << ms(t0,t1) << " ms" << endl;

    // --- 2.4 Doi chieu voi brute-force (gop ngau nhien)
    DSU d2;
    vector<int> label(N);
    for (int i = 0; i < N; i++) { label[i] = i; d2.addContainer(ids[i]); }
    mt19937 rng(12345);
    for (int k = 0; k < 12000; k++) {
        int a = rng()%N, b = rng()%N;
        d2.unite(ids[a], ids[b]);                                // Buoc 2: chay DSU
        int la = label[a], lb = label[b];                        // doi chung bang mang nhan
        if (la != lb) for (int i = 0; i < N; i++) if (label[i]==lb) label[i]=la;
    }
    bool match = true;                                           // Buoc 3
    for (int k = 0; k < 20000 && match; k++) {
        int a = rng()%N, b = rng()%N;
        if (d2.connected(ids[a], ids[b]) != (label[a]==label[b])) match = false;
    }
    CHECK("Test 19: 12000 unite ngau nhien + 20000 truy van khop brute-force", match);
    bool sizeOk = true;
    for (int k = 0; k < 300 && sizeOk; k++) {
        int a = rng()%N, cnt = 0;
        for (int i = 0; i < N; i++) if (label[i]==label[a]) cnt++;
        if ((int)d2.getLinkedContainers(ids[a]).size() != cnt) sizeOk = false;
    }
    CHECK("Test 20: Kich thuoc nhom khop brute-force (300 mau)", sizeOk);

    // --- 2.5 150 lo, moi lo 100 container
    DSU d3;
    for (auto &s : ids) d3.addContainer(s);
    t0 = chrono::steady_clock::now();
    for (int i = 0; i < N; i++) if (i % 100 != 0) d3.unite(ids[i - i%100], ids[i]);
    t1 = chrono::steady_clock::now();
    CHECK("Test 21: Cung lo (0..99) lien thong", d3.connected(ids[0], ids[99]));
    CHECK("Test 22: Khac lo (99 va 100) KHONG lien thong", !d3.connected(ids[99], ids[100]));
    CHECK("Test 23: Moi lo co dung 100 container", d3.getLinkedContainers(ids[250]).size() == 100);
    cout << "     -> 150 lo x 100 container: " << ms(t0,t1) << " ms" << endl;

    // --- 2.6 Mo phong groupContainersByDeclaration: 3000 to khai x 5 container
    {
        DSU d4;
        unordered_map<string, string> declToFirst;
        t0 = chrono::steady_clock::now();
        for (int i = 0; i < N; i++) {
            d4.addContainer(ids[i]);
            string tk = "TK_" + to_string(i % 3000);
            auto it = declToFirst.find(tk);
            if (it != declToFirst.end()) d4.unite(it->second, ids[i]);
            else declToFirst[tk] = ids[i];
        }
        t1 = chrono::steady_clock::now();
        CHECK("Test 24: Cung ma to khai (i va i+3000) lien thong",
              d4.connected(ids[7], ids[3007]) && d4.connected(ids[7], ids[12007]));
        CHECK("Test 25: Khac ma to khai KHONG lien thong", !d4.connected(ids[7], ids[8]));
        bool ok5 = true;
        for (int i = 0; i < N; i += 97) if (d4.getLinkedContainers(ids[i]).size() != 5) ok5 = false;
        CHECK("Test 26: Moi nhom to khai co dung 5 container", ok5);
        CHECK("Test 27: Gop tay 2 nhom => unite true, nhom 10 container",
              d4.unite(ids[0], ids[1]) && d4.getLinkedContainers(ids[1]).size() == 10);
        cout << "     -> mo phong gom nhom theo to khai: " << ms(t0,t1) << " ms" << endl;
    }

    cout << "\n==========================================" << endl;
    cout << "KET QUA: " << passed_tests << "/" << total_tests << " test dat" << endl;
    cout << (passed_tests == total_tests ? "TAT CA DEU [PASS]" : "CO TEST [FAIL] - can debug!") << endl;
    return passed_tests == total_tests ? 0 : 1;
}