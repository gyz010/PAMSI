//
// Created by gyz on 06.05.24.
//

#ifndef C3_3_CHECKERS_H
#define C3_3_CHECKERS_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include "BoardActions.h"

enum GameResult {NONE, BLACK_WIN, WHITE_WIN, DRAW};

class Checkers {
    // 0: empty, 1: black, -1: white 2: black king, -2: white king
    // black starts first
    // direction of move = sign(position)

    int turn;
    int no_jumps_count;
    GameResult result;
    void draw_board();

    bool is_king(const position &pos);
    bool is_valid_square(const position &pos);
    bool is_legal_move(const position &from, const position &to);
    bool move(const position &from, const position &to);
    bool jump(const position &from, const position &to);
    bool action(const std::string &notation);
    std::vector<position> available_moves();
    std::vector<position> available_jumps();
    bool checkWinCondition();
    void available_jumps_seq_recursion(const position &from, std::vector<position> &seq, std::vector<std::vector<position>> &result);
protected:
    std::vector<std::vector<int>> board;
    std::vector<position> available_moves_from(const position &from, int piece);
    std::vector<position> available_jumps_from(const position &from, int piece);
    std::vector<std::vector<position>> available_jumps_sequence(const position &from, int piece);
public:

    explicit Checkers(std::vector<std::vector<int>> &board);
    void play();

    //Getters
    int getTurn() const;
    static std::vector<position> convert_notation(const std::string &notation);

};

#endif //C3_3_CHECKERS_H
