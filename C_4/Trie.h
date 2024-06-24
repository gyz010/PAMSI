//
// Created by gyz on 23.06.24.
//

#ifndef C_4_TRIE_H
#define C_4_TRIE_H

#include <unordered_map>

class Trie {
private:
    struct Node {
        std::unordered_map<char, Node*> children;
        bool is_end;
        Node() {
            is_end = false;
        }
    };
private:
    Node *root;

public:
    Trie() {root = new Node();}



};


#endif //C_4_TRIE_H
