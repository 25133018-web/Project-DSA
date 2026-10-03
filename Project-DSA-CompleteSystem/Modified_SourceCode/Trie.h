#pragma once

#include "Container.h"
#include "vector"

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
  void dfs(TrieNode *node, string currentprefix, vector<Container *> &result) {
    if (node->isEnd && node->container) {
      result.push_back(node->container);
    }
    for (int i = 0; i < 36; i++) {
      if (node->children[i]) {
        dfs(node->children[i], currentprefix + getChar(i), result);
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
      if (index == -1 || curr->children[index] == nullptr)
        continue;
      if (curr->children[index] == nullptr) {
        return results; // khong tim ra
      }
      curr = curr->children[index];
    }
    dfs(curr, prefix, results);
    return results;
  }
  // auto complete function
  vector<Container *> searchbyprefix(const string &fprefix) {
    string prefix = fprefix;
    for (char &c : prefix) {
      c = toupper(c);
    }
    if (prefix.size() < 2) {
      cout << "Prefix must be at least 2 characters long.\n";
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