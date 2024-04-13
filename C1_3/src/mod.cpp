//
// Created by gyz on 27.03.24.
//

#include "../inc/mod.h"

void packet_assembler::insert_recursive(const std::string &data, const size_t &packet_index, Node *node) {
    if (packet_index < node->packet_index) {
        if (!(node->left)) {
            node->left = new Node (nullptr, nullptr, data, packet_index);
            return;
        } else {
            insert_recursive(data, packet_index, node->left);
        }
    }
    if (packet_index > node->packet_index) {
        if (!(node->right)) {
            node->right = new Node (nullptr, nullptr, data, packet_index);
            return;
        } else {
            insert_recursive(data, packet_index, node->right);
        }
    }
    if (packet_index == node->packet_index) {
        node->message = data;
        return;
    }
}

void packet_assembler::in_order_printout_recursive(Node* node) {
    if(!node) return;

    if(node->left) {
        in_order_printout_recursive(node->left);
    }
    std::cout << node->message;

    if(node->right) {
        in_order_printout_recursive(node->right);
    }
}
void packet_assembler::clear_recursive(Node *node) {
    if(!node) return;

    clear_recursive(node->left);
    clear_recursive(node->right);

    delete node;
}
int* packet_assembler::create_random_order_array(const size_t &array_size) {
    int* packet_order = new int[array_size];
    std::random_device rand;
    for(size_t i=0; i<array_size; i++) {
        packet_order[i]=i;
    }
    int buf;
    for(size_t i=array_size; i>1; i--) {
        size_t random_number = rand()%i;
        buf = packet_order[random_number];
        packet_order[random_number] = packet_order[0];
        packet_order[0] = buf;
    }
    return packet_order;
}