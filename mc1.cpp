#include <iostream>
#include <string>
#include <cctype>
using namespace std;
const int TABLE_SIZE = 20011;
const int MAX_container = 15000;
struct Node {
    Container data;
    Node* next;
    Node(const Container& c) : data(c), next(nullptr) {}
};

Node* hashTable[TABLE_SIZE] = {nullptr};
void normalizeID(string &id) {
    for (char &c : id) {
        c = static_cast<char>(toupper(c));
    }
}

int hashFunction(string container_id) {
    normalizeID(container_id);
    unsigned long hash = 5381;
    for (char c : container_id) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return static_cast<int>(hash % TABLE_SIZE);
}
void insertToHashTable(Container c) {
    normalizeID(c.container_id);
    int index = hashFunction(c.container_id);
    Node* curr = hashTable[index];
    while (curr != nullptr) {
        if (curr->data.container_id == c.container_id) {
            return ; 
        }
        curr = curr->next;
    }
    Node* newNode = new Node(c);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}
bool remove(string target_id) {
    normalizeID(target_id); 
    int index = hashFunction(target_id);

    Node* curr = hashTable[index];
    Node* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.container_id == target_id) {
            if (prev == nullptr) {
                hashTable[index] = curr->next;
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

void SEARCH_ID() {
    string target_id;
    cout << "\n==========================================";
    cout << "\nNhap vao ID Container can tim kiem: ";
    cin >> target_id;
    if(target_id.length() != 11){
        cout << "Loi.Ma container phai gom dung 11 ky tu!";
        return;
    }
    normalizeID(target_id);
    int index = hashFunction(target_id);
    Node* curr = hashTable[index];

    bool found = false;
    while (curr != nullptr) {
        if (curr->data.container_id == target_id) {
            cout << "Thong tin container mang ID : " << target_id;
            displayContainer(curr->data);
            found = true;
            break;
        }
        curr = curr->next;
    }

    if (!found) {
        cout << "\nKhong tim thay Container mang ID " << target_id << "\n";
    }
}
