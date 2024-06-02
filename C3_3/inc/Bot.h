//
// Created by gyz on 20.05.24.
//

#ifndef C3_3_BOT_H
#define C3_3_BOT_H

#include <vector>
#include <string>
#include "Checkers.h"
#include <random>
#include <limits>

class Bot {
private:
    struct Node {
        double eval;
        std::string move;
        std::vector<Node> children;
        int turn=0;
    };
    const int depth;
    const int64_t seed;
    const bool is_black;
    std::mt19937_64 gen;
    void removeChildren(Node &node);
public:
    explicit Bot(bool is_black, int depth, int64_t seed) : is_black(is_black),
                                                  depth(depth),
                                                  seed(seed),
                                                  gen(std::mt19937_64(seed)) {};
    Node root;
    void fillTree(Node &node, int depth, std::vector<std::vector<int>> temp_board, int turn);
    void collapseTree(std::string &notation);
    std::string getBestMove();
    double evaluate(Node &node, const std::vector<std::vector<int>> &board);
    void minmax(Node &node, int depth, bool is_max, double alpha, double beta, std::vector<std::vector<int>> board);

    };

#endif //C3_3_BOT_H
