//
// Created by gyz on 08.03.24.
//

#ifndef C1_3_MOD_H
#define C1_3_MOD_H

#include <iostream>
#include <exception>
#include <random>


class packet_assembler {
private:
    struct Node {
        Node* left;
        Node* right;
        std::string message;
        size_t packet_index;

        Node(Node* l, Node* r, std::string  m, const size_t i) :
                left(l), right(r), message(std::move(m)), packet_index(i) {}
    };

    Node *root;
private:

    /**
     * @brief Recursively inserts element into sorted binary tree.
     *
     * @param data Data to insert.
     * @param packet_index Sorting index.
     * @param node Root of the subtree.

     * @return void.
     */
    void insert_recursive(const std::string &data, const size_t &packet_index, Node *node);


    /**
     * @brief Inorder tree traversal with data printing.
     *
     * @param node Root of the subtree.
     * @return void.
     */
    void in_order_printout_recursive(Node* node);


    /**
     * @brief Destroys every element and deallocates its memory.
     *
     * @return void.
     */
    void clear_recursive(Node *node);

public:

    /**
     * @brief Constructor initializing root as a nullptr
     */
    packet_assembler() noexcept : root(nullptr) {};

    /**
     * @brief Creates an array of array_size different integers.
     *
     * @param array_size size of created array.
     * @return pointer to created array, int*
     */
    static int* create_random_order_array(const size_t &array_size);

    /**
     * @brief Inserts element into sorted binary tree.
     *
     * @param data Data to insert.
     * @param idx Sorting index.
     * @return void.
     */
    void insert_packet(const size_t &idx, const std::string &data) {
        if(!root) {
            root = new Node (nullptr, nullptr, data, idx);
            return;
        }
        else {
            insert_recursive(data, idx, root);
        }
    }
    /**
     * @brief Inserts and shuffles elements of array.
     *
     * @param array Pointer to array of
     * @param idx Sorting index.
     * @return void.
     */
    void shuffle_and_insert_packets(const std::string* array, const size_t &array_size) {
        int* packet_order = create_random_order_array(array_size);
        for(size_t i=0; i<array_size; i++) {
            insert_packet(packet_order[i], array[packet_order[i]]);
        }
        delete[] packet_order;
    }
    void insert_packets(const std::string* array, const size_t &array_size, int* packet_order) {
        for(size_t i=0; i<array_size; i++) {
            insert_packet(packet_order[i], array[packet_order[i]]);
        }
    }
    /**
     * @brief Prints out sorted packets.
     * @return void
     */
    void in_order_printout() {
        in_order_printout_recursive(root);
    }
    /**
     * @brief Destroys every element and deallocates its memory.
     * @return void
     */
    void clear() {
        clear_recursive(root);
        root = nullptr;
    }
};

#endif //C1_3_MOD_H
