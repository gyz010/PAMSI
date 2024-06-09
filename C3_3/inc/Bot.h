//
// Created by gyz on 20.05.24.
//

#ifndef C3_3_BOT_H
#define C3_3_BOT_H

#include <vector>
#include <string>
#include <random>
#include <limits>
#include <chrono>
#include <thread>
#include "../inc/Evaluation.h"

enum evaluation {RANDOM, SIMPLE, TWENTY_FIVE_PARAMS};

class Bot {
private:
    struct Node {
        double eval=0;
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
    //Funkcja budowania drzewa ruchów
    void fillTree(Node &node, int depth, std::vector<std::vector<int>> &board, int turn);

    //Minmax z cięciami alfa-beta
    void minmax(Node &node, int depth, bool is_max, double alpha, double beta, std::vector<std::vector<int>> &board);

    //Połączenie funkcji fillTree i minmax w celu optymalizacji
    void fill_tree_with_minmax(Node &node, std::vector<std::vector<int>> &board, int depth, int turn, bool is_max, double alpha, double beta);

    //Usuwanie dzieci nodea
    void removeChildren(Node &node);

    //Usunięcie całego drzewa, nadanie rootowi ostatni ruch
    void collapseTree(const std::string &notation);

    //Zwraca najlepszy ruch
    std::string getBestMove();
public:
    explicit Bot(bool is_black, int depth, int64_t seed, evaluation eval) : is_black(is_black),
                                                                            depth(depth),
                                                                            gen(std::mt19937_64(seed)),
                                                                            root("", 0),
                                                                            eval_type(eval){};

    std::string move(std::vector<std::vector<int>> &board, int turn, const std::string &notation);

    //Ustawienie parametrów dla funkcji heurystycznej
    void setParameters(const std::vector<double> &parameters) { this->parameters = parameters;}

    //Zmiana strony (do uczenia parametrów)
    void switchSide() { is_black = !is_black; }
    double getEval() const { return root.eval; }
    void clearRoot() { root.children.clear(); }
};

#endif //C3_3_BOT_H