//
// Created by gyz on 20.05.24.
//

#ifndef C3_3_BOT_H
#define C3_3_BOT_H

#include <vector>
#include <string>
#include "Checkers.h"

class Bot : protected Checkers {
private:
    struct Node {
        double self_eval;
        std::vector<std::string> move;
        std::vector<Node> children;
    };
    Node root;
    const int depth;
public:
    explicit Bot(int depth, std::vector<std::vector<int>> &board) : Checkers(
            board), depth(depth) {}
    void fillTree(Node &node, int depth, std::vector<std::vector<int>> temp_board, bool black_to_move);
    void giveMove();
    double evaluate();
};


#endif //C3_3_BOT_H
