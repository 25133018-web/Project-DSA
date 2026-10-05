#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// ============================================================================
// 1. CẤU TRÚC DỮ LIỆU CONTAINER & ENUM (Mô phỏng thay thế Container.h)
// ============================================================================
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

// ============================================================================
// 2. CÂY TIỀN TỐ CONTAINER TRIE (Nội dung gốc từ Trie.h)
// ============================================================================
struct TrieNode {
  TrieNode *children[36];
  Container *container;
  bool isEnd;
  TrieNode() {
    isEnd = false;
    container = nullptr;
    for (int i = 0; i < 36; i++) {
      children[i] = nullptr;
    }
  }
};

class ContainerTrie {
private:
  TrieNode *root;

  int getIndex(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
      return ch - 'A';
    } else if (ch >= '0' && ch <= '9') {
      return ch - '0' + 26;
    }
    return -1;
  }

  char getChar(int index) {
    if (index >= 0 && index < 26) {
      return 'A' + index;
    } else if (index >= 26 && index < 36) {
      return '0' + (index - 26);
    }
    return '\0'; // Invalid index
  }

  void dfs(TrieNode *node, string& currentprefix, vector<Container*> &result) {
    if (node->isEnd && node->container) {
      result.push_back(node->container);
    }
    for (int i = 0; i < 36; i++) {
      if (node->children[i]) {
        currentprefix.push_back(getChar(i));
        dfs(node->children[i], currentprefix, result);
        currentprefix.pop_back(); // quay tro lai 
      }
    }
  }

  void clearNode(TrieNode *node) {
    if (!node)
      return;
    for (int i = 0; i < 36; i++) {
      if (node->children[i]) {
        clearNode(node->children[i]);
      }
    }
    delete node;
  }

public:
  ContainerTrie() { root = new TrieNode(); }
  ~ContainerTrie() { clearNode(root); }
  ContainerTrie(const ContainerTrie &) = delete;
  ContainerTrie &operator=(const ContainerTrie &) = delete;

  void insert(Container *c) {
    TrieNode *curr = root;
    for (char ch : c->container_id) {
      int index = getIndex(ch);
      if (index == -1)
        continue; // skipinvalidcharacters
      if (curr->children[index] == nullptr) {
        curr->children[index] = new TrieNode();
      }
      curr = curr->children[index];
    }
    curr->isEnd = true;
    curr->container = c;
  }

  vector<Container *> getSuggestions(const string &prefix) {
    TrieNode *curr = root;
    vector<Container *> results;
    for (char ch : prefix) {
      int index = getIndex(ch);
      if (index == -1)
        return results; // bo qua ki tu khong hop le(la)
      if (curr->children[index] == nullptr) {
        return results; // nhanh khong ton tai 
      }
      curr = curr->children[index];
    }
    string buffer = prefix;
    dfs(curr, buffer, results);
    return results;
  }

  // auto complete function
  vector<Container *> searchbyprefix(const string &fprefix) {
    string prefix = fprefix;
    for (char &c : prefix) {
      c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    if (prefix.size() < 2) {
      return {};
    }
    return getSuggestions(prefix);
  }

  void showInfo(Container *c) {
    if (!c) {
      cout << "  [!] Container khong ton tai.\n";
      return;
    }
    cout << "  Container ID : " << c->container_id << "\n";
    cout << "  Label         : " << labelToString(c->container_label) << "\n";
    cout << "  Status    : " << statusToString(c->status) << "\n";
    cout << "  Gross weight (kg) : " << c->gross_weight << "\n";
    cout << "  Ma to khai      : " << c->customs_declaration_no << "\n";
  }
};

// ============================================================================
// 3. KHUNG HÀM CHECK() THỜI GIAN THỰC THEO HƯỚNG DẪN D5
// ============================================================================
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

// ============================================================================
// 4. HÀM MAIN THỰC THI BỘ KIỂM THỬ TỰ ĐỘNG
// ============================================================================
int main() {
    ContainerTrie trie;

    // --- BƯỚC 1: CHUẨN BỊ DỮ LIỆU MẪU ---
    Container c1 = {"MAEU1234567", Label_Container::GP, Status_Container::in_yard, 24.5, "TK001"};
    Container c2 = {"MAEU9999999", Label_Container::DANGER, Status_Container::in_yard, 30.0, "TK002"};
    Container c3 = {"CBHU1234567", Label_Container::REEFER, Status_Container::exported, 18.2, "TK003"};

    // --- BƯỚC 2: NẠP DỮ LIỆU VÀO TRIE ---
    trie.insert(&c1);
    trie.insert(&c2);
    trie.insert(&c3);

    // --- BƯỚC 3: CHẠY BỘ TEST CASE KIỂM TRA ---

    // Test 1: Tìm gợi ý đúng tiền tố hợp lệ
    vector<Container*> res1 = trie.searchbyprefix("MAEU");
    CHECK("Test 1: Tim thay du 2 container voi tien to 'MAEU'", res1.size() == 2);

    // Test 2: Gợi ý không phân biệt chữ hoa / chữ thường
    vector<Container*> res2 = trie.searchbyprefix("cbhu");
    CHECK("Test 2: Tim chuoi thuong 'cbhu' tu dong chuyen thanh 'CBHU'", res2.size() == 1 && res2[0]->container_id == "CBHU1234567");

    // Test 3: Điều kiện biên - Tiền tố ngắn hơn 2 ký tự
    vector<Container*> res3 = trie.searchbyprefix("M");
    CHECK("Test 3: Tien to ngan (< 2 ky tu) tra ve danh sach rong", res3.empty() == true);

    // Test 4: Tiền tố không tồn tại
    vector<Container*> res4 = trie.searchbyprefix("TEXU");
    CHECK("Test 4: Tien to khong ton tai 'TEXU' tra ve danh sach rong", res4.empty() == true);

    // Test 5: Tiền tố chứa ký tự không hợp lệ (ký tự đặc biệt)
    vector<Container*> res5 = trie.searchbyprefix("MA@#");
    CHECK("Test 5: Tien to chua ky tu dac biet tra ve danh sach rong", res5.empty() == true);

    cout << "\n--- Kiem tra showInfo() ---" << endl;
    cout << "Goi showInfo voi container hop le:" << endl;
    trie.showInfo(&c1);
    cout << "Goi showInfo voi con tro nullptr (Khong duoc crash):" << endl;
    trie.showInfo(nullptr);
    CHECK("Test 7: showInfo xu ly an toan voi con tro nullptr", true);

    // --- TỔNG KẾT KẾT QUẢ ---
    cout << "   KET QUA: " << passed_tests << "/" << total_tests << " TEST CASES PASSED!" << endl;
    if (passed_tests == total_tests) {
        cout << "[PASS]" << endl;
    } else {
        cout << "[FAIL]" << endl;
    }
    return 0;
}