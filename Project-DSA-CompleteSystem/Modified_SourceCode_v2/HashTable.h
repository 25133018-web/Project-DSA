#pragma once

#include "Container.h"
#include <string>

using namespace std;

const int TABLE_SIZE = 20011;

struct HashNode {
  Container data;
  HashNode *next;
  HashNode(const Container &c) : data(c), next(nullptr) {}
};

class HashTable {
private:
  HashNode *table[TABLE_SIZE];

  // Hàm băm (thuật toán djb2 của bạn mc1)
  int hashFunction(const string &container_id) const {
    unsigned long hash = 5381;
    for (char c : container_id) {
      hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return static_cast<int>(hash % TABLE_SIZE);
  }

public:
  // Constructor: Khởi tạo tất cả các bucket ban đầu là nullptr
  HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
      table[i] = nullptr;
    }
  }

  // Destructor: Tự động dọn rác, delete toàn bộ Node khi tắt chương trình
  // (chống rò rỉ bộ nhớ)
  ~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
      HashNode *curr = table[i];
      while (curr != nullptr) {
        HashNode *temp = curr;
        curr = curr->next;
        delete temp;
      }
      table[i] = nullptr;
    }
  }

  // Thêm một container vào bảng băm
  void insert(const Container &c) {
    int index = hashFunction(c.container_id);
    HashNode *newNode = new HashNode(c);
    newNode->next = table[index];
    table[index] = newNode;
  }

  // Tìm kiếm container theo ID: Trả về con trỏ tới Container tìm thấy (hoặc
  // nullptr nếu không có)
  Container *search(const string &target_id) const {
    int index = hashFunction(target_id);
    HashNode *curr = table[index];

    while (curr != nullptr) {
      if (curr->data.container_id == target_id) {
        return &(curr->data); // Trả về địa chỉ của container tìm thấy
      }
      curr = curr->next;
    }
    return nullptr; // Không tìm thấy
  }

  bool remove(const string &target_id) {
    int index = hashFunction(target_id);
    HashNode *curr = table[index];
    HashNode *prev = nullptr;
    while (curr != nullptr) {
      if (curr->data.container_id == target_id) {
        if (prev == nullptr) {
          table[index] = curr->next;
        } else {
          prev->next = curr->next;
        }
        delete curr;
        return true;
      }
      prev = curr;
      curr = curr->next;
    }
    return false;
  }
};
