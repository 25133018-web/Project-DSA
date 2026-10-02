#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct TrieNode{
    TrieNode* children[36];
    bool isEnd;
    TrieNode(){
        isEnd = false;
        for (int i = 0; i < 36; i++){
            children[i] = nullptr;
        }
    }
};
class ContainerTrie{

    private:
    struct Trie{
        TrieNode* children[36];
        bool isEnd;

        Trie(){
            isEnd = false;
            for (int i = 0; i < 36; i++){
                children[i] = nullptr;
            }
        }
    };
    TrieNode* root;
    int getIndex(char ch){
        if (ch >= 'A' && ch <= 'Z'){
            return ch - 'A';
        } else if (ch>='0' && ch <= '9'){
            return ch - '0' + 26;
        }
        return -1; 
    }
    char getChar(int index){
        if (index >= 0 && index < 26){
            return 'A' + index;
        } else if (index >= 26 && index < 36){
            return '0' + (index - 26);
        }
        return '\0'; // Invalid index
    }
    void dfs(TrieNode* node, string& currentprefix, vector<string>results){
        if (node->isEnd){
            results.push_back(currentprefix);
        }
        for (int i=0; i<36;i++){
            if (node->children[i]){
                dfs(node->children[i], currentprefix + getChar(i), results);
            }
        }
    }
    void clearNode(){
        if(!node) return; 
        for(int i=0; i<36; i++){
            if (node->children[i]){
                clearNode(node->children[i]);
            }
        }
        delete node;
    }
    public:
    Tries(){
        root = new TrieNode();
    }
    ~Tries(){
        clearNode(root);
    }
    void insert(const string& word){
        TriesNode* curr = root;
        for (char ch:word){
            int index = getIndex(ch);
            if (index == -1) continue; 
            if (curr->children[index] == nullptr){
                curr->children [index] = new TrieNode();
            }
            curr = curr->children[index];
        }
        curr->isEnd = true;
    }
    vector<string> getSuggestions(const string& prefix){
        TrieNode* curr = root;
        vector<string> results;
        for (char ch: word){
            int index = getIndex(ch);
            if (index == -1 ||curr->children[index] == nullptr) continue; 
            if (curr->children[index] == nullptr){
                return results; //khong tim ra
            }
            curr = curr->children[index];
        }
        dfs(curr,prefix,results);
        return results;
    }

    //dit me tuoi lon 







    // public:
    //     ContainerTrie(){
    //         root = new TrieNode();
    //     }
    //     vector<char> suggestNextChar(const string& prefix){
    //         TrieNode* current = root;
    //         vector <char> nextChars;
    //         for ( char ch : prefix){
    //             ch = toupper(ch); // in hoa 
    //             if (current->children.find(ch)==current->children.end())
    //         }
    //     }
}
