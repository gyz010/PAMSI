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
#include <chrono>
#include <thread>
#include "../inc/Evaluation.h"

enum evaluation {RANDOM, SIMPLE, ELEVEN_PARAMS, TWENTY_FIVE_PARAMS};

class Bot {
private:
    struct Node {
        double eval;
        std::string move;
        std::vector<Node> children;
        int turn;
        explicit Node(std::string move, int turn) : move(std::move(move)), turn(turn) {};
    };
private:
    const int depth;
    bool is_black;
    std::mt19937_64 gen;
    std::vector<double> parameters;
    evaluation eval_type;
    Node root;
private:
    void removeChildren(Node &node);
    void fillTree(Node &node, int depth, std::vector<std::vector<int>> &board, int turn);
    void collapseTree(const std::string &notation);
    std::string getBestMove();
    double evaluate(const std::vector<std::vector<int>> &board);
    void minmax(Node &node, int depth, bool is_max, double alpha, double beta, std::vector<std::vector<int>> &board);
    void minmax_25(Node &node, int depth, bool is_max, double alpha, double beta, std::vector<std::vector<int>> &board);
public:
    explicit Bot(bool is_black, int depth, int64_t seed, evaluation eval) : is_black(is_black),
                                                                            depth(depth),
                                                                            gen(std::mt19937_64(seed)),
                                                                            root("", 0),
                                                                            eval_type(eval){};
    std::string move(std::vector<std::vector<int>> &board, int turn, const std::string &notation);
    void setParameters(const std::vector<double> &parameters) { this->parameters = parameters;}
    void switchSide() { is_black = !is_black;}
};

#endif //C3_3_BOT_H
