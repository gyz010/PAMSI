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
#include "Bot.h"

enum GameResult {NONE, BLACK_WIN, WHITE_WIN, DRAW};

class Checkers {
    // direction of move = sign(board[position])
    int turn;

    // 0: empty, 1: black, -1: white 2: black king, -2: white king
    // black starts first
    std::vector<std::vector<int>> board;
    int no_jumps_count;
    GameResult result;
    void draw_board();
    bool check_win_condition();
public:
    explicit Checkers(std::vector<std::vector<int>> &board);

    void play();
    void play_with_bot(bool is_human_black, int depth, int64_t seed);
    void bot_vs_bot();

    //Getters
    int getTurn() const;

};

#endif //C3_3_CHECKERS_H
