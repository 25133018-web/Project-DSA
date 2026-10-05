#pragma once

#include "DSU.h"
#include "HashTable.h"
#include "PriorityQueue.h"
#include "Trie.h"
#include <stdexcept>
#include <unordered_map>

// Hàm tính số lượng container hiện có trong bãi trước khi tiến hành nhập
// container(s) ngoài cổng (đừng quan tâm hàm này)
int CountContainersInYard(const unordered_map<string, Vessel> &VesselMap);

// Hàm nhập container(s) ngoài cổng vào bãi
void AddContainer(unordered_map<string, Vessel> &VesselMap,
                  list<Container> &GateContainerQueues,
                  HashTable &yardHashTable, ContainerTrie &yardTrie,
                  DSU &yardDSU,
                  unordered_map<string, Container *> &containerLookup);

void ExportContainer(string &container_id,
                     unordered_map<string, Vessel> &VesselMap,
                     priority_queue &ExportQueue, HashTable &yardHashTable,
                     ContainerTrie &yardTrie,
                     unordered_map<string, Container *> &containerLookup);

void Interact(unordered_map<string, Vessel> &VesselMap,
              priority_queue &ExportQueue, DSU &yardDSU,
              unordered_map<string, Container *> &containerLookup,
              HashTable &yardHashTable, ContainerTrie &yardTrie);

void PrintOrderedExportedContainerQueue(priority_queue &ExportQueue);

void displayContainer(const Container &c);

void groupContainersByDeclaration(
    unordered_map<string, Vessel> &VesselMap, DSU &dsu,
    unordered_map<string, Container *> &containerLookup);

void XuatThongTinCungMaToKhai(
    DSU &dsu, const unordered_map<string, Container *> &containerLookup,
    const string &target_id);

void SEARCH_ID(const HashTable &yardHashTable, ContainerTrie &yardTrie);

void gopNhomContainer(DSU &dsu,
                      unordered_map<string, Container *> &containerLookup);
