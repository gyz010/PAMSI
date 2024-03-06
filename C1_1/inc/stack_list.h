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
    size_t size;
public:
    stack_list() {
        node=nullptr;
        size=0;
    }
    bool empty() {
        return (!size);
    }
    void push(const T &data) {
        try {
            if (node) {
                node->next = new Node;
                node->next->prev = node;
                node->next->val = data;
                node = node->next;
            } else {
                node = new Node;
                node->val = data;
                node->next = nullptr;
                node->prev = nullptr;
            }
            ++size;
        }
        catch (std::bad_alloc &e) {
            std::cout << e.what() << std::endl;
        }
    }
    void pop() {
        if(empty()) {
            throw (std::runtime_error("Stack is empty"));
        }
        if(node->prev) {
            node = node->prev;
            delete node->next;
            node->next = nullptr;
        }
        else {
            delete node;
            node = nullptr;
        }
        --size;
    }
    T peek() {
        if(empty()) {
            throw (std::runtime_error("Stack is empty"));
        }
        return node->val;
    }
    void clear() {
        while(size>0) {
            node=node->prev;
            delete node->next;
        }
        if (node) {
            delete node;
            node = nullptr;
        }
    }


};

#endif //C1_1_STACK_LIST_H
