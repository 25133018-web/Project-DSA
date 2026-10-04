#pragma once

#include "Container.h"
#include <string>
#include <cctype>

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
  static void normalizeID(string &id) {
        for (char &c : id) {
            c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }
    }

  // Hàm băm (thuật toán djb2 của bạn mc1)
 int hashFunction(string container_id) const {
        normalizeID(container_id);
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
void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode *curr = table[i];
            while (curr != nullptr) {
                HashNode *temp = curr;
                curr = curr->next;
                delete temp;
            }
            table[i] = nullptr; // Chống dangling pointer
        }
    }

  // Thêm một container vào bảng băm
  bool insert(const Container &c) {
        Container temp = c;
        normalizeID(temp.container_id);
        int index = hashFunction(temp.container_id);

        HashNode *curr = table[index];
        while (curr != nullptr) {
            if (curr->data.container_id == temp.container_id) {
                return false; // ID đã tồn tại trong hệ thống
            }
            curr = curr->next;
        }

        HashNode *newNode = new HashNode(temp);
        newNode->next = table[index];
        table[index] = newNode;
        return true;
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
};
// Xóa một container khỏi bảng băm theo ID
    bool remove(string target_id) {
        normalizeID(target_id);
        int index = hashFunction(target_id);

        HashNode *curr = table[index];
        HashNode *prev = nullptr;

        while (curr != nullptr) {
            if (curr->data.container_id == target_id) {
                if (prev == nullptr) {
                    table[index] = curr->next; // Xóa Node đầu danh sách
                } else {
                    prev->next = curr->next;   // Xóa Node ở giữa/cuối
                }
                delete curr; // Thu hồi bộ nhớ
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }
};
