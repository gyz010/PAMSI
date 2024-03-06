//
// Created by Asus on 06.03.2024.
//

#ifndef C1_1_STACK_LIST_H
#define C1_1_STACK_LIST_H

#include<memory>
#include<iostream>

template <typename T>
struct Node {
    Node* next;
    Node* prev;
    T val;
};

template <typename T, typename Node=Node<T>>
class stack_list {
private:
    Node* node;
public:
    stack_list() {
        node=nullptr;
    }
    void push(const T &data) {
        if (node) {
            node->next = new Node;
            node->next->prev = node;
            node->next->val = data;
            node = node->next;
        }
        else {
            node = new Node;
            node->val = data;
            node->next = nullptr;
            node->prev = nullptr;
        }
    }
    void pop() {
        if(node->prev) {
            node = node->prev;
            delete node->next;
            node->next = nullptr;
        }
        else {
            delete node;
            node = nullptr;
        }
    }
    T peek() {
        return node->val;
    }
    bool empty() {
        return (!node);
    }


};

#endif //C1_1_STACK_LIST_H
