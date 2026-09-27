const int TABLE_SIZE = 1007;

struct Node {
    Container data;
    Node* next;
    Node(const Container& c) : data(c), next(nullptr) {}
};

Node* hashTable[TABLE_SIZE] = {nullptr};

int hashFunction(const string& container_id) {
    unsigned long hash = 5381;
    for (char c : container_id) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TABLE_SIZE;
}

void insertToHashTable(const Container& c) {
    int index = hashFunction(c.container_id);
    Node* newNode = new Node(c);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

void SEARCH_ID() {
    string target_id;
    cout << "\n==========================================";
    cout << "\nNhap vao ID Container can tim kiem: ";
    cin >> target_id;

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
