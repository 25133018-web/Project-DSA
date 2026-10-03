#pragma once

#include "Container.h"
#include "unordered_map"
#include <vector>

// gom nhom bang dsu //đoạn Hào mới sửa từ đây
class DSU {
private:
  unordered_map<string, string> parent;
  unordered_map<string, int> rankMap;
  unordered_map<string, vector<string>> groupMembers;
public:
  // thao tac them container vao DSU neu chua co
  void addContainer(const string &containerID) { // truyen vao id container
    if (parent.find(containerID) == parent.end()) {
      parent[containerID] = containerID;
      rankMap[containerID] = 0;
      groupMembers[containerID] = {containerID};
    }
  }
  bool contains(const string &containerID) const {
    return parent.find(containerID) != parent.end();
  }
  // thao tac find
  string find(const string &containerID) {
    // tu dong khoi tạo neu chua ton tai
    if (parent.find(containerID) == parent.end()) {
      addContainer(containerID);
      return containerID;
    }
    // pass1: find root
    string root = containerID;
    while (parent[root] != root) {
      root = parent[root];
    }
    // pass 2: nén đường đi
    string current = containerID;

    while (current != root) {
      string next = parent[current];
      parent[current] = root;
      current = next;
    }
    return root;
  }

  bool unite(const string& containerA, const string& containerB){
			string rootA = find(containerA);
			string rootB = find(containerB);
			if (rootA == rootB) return false;
			// Xac dinh newRoot (gốc mới) va childRoot (gốc cũ bị gộp vào)
        	string newRoot = rootA;
        	string childRoot = rootB;

      if (rankMap[rootA] < rankMap[rootB]) {
            	newRoot = rootB;
            	childRoot = rootA;
      } 
		  else if (rankMap[rootA] == rankMap[rootB]) {
           	 	rankMap[rootA]++;
      }

      parent[childRoot] = newRoot;

      // Chuyen toan bo thanh vien tu childRoot sang newRoot
      auto& listNew = groupMembers[newRoot];
      auto& listChild = groupMembers[childRoot];
      listNew.insert(listNew.end(), make_move_iterator(listChild.begin()), make_move_iterator(listChild.end()));
      groupMembers.erase(childRoot);

      return true;
		}

		// kiem tra 2 container co cung nhom khong
		bool connected(const string& a, const string& b){
			if (!contains(a) || !contains(b)) return false;
			return find(a) == find(b);
		}

		// Truy van O(1) danh sach container cung nhom
    vector<string> getLinkedContainers(const string& id) {
        if (!contains(id)) return {};
        string root = find(id);
        return groupMembers[root];
    }
};
