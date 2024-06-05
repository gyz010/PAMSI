#include <iostream>
#include "../inc/Checkers.h"
int main() {
    enum interface {NET, GUI};
    int64_t seed = 0;
    bool is_black = true;
    int depth = 9;
    interface game_type = GUI;


    std::vector<std::vector<int>> board = { { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 1, 0,  1, 0,  1, 0,  1, 0},
                                            { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            {-1, 0, -1, 0, -1, 0, -1, 0},
                                            { 0,-1,  0,-1,  0,-1,  0,-1},
                                            {-1, 0, -1, 0, -1, 0, -1, 0} };

/*  std::vector<std::vector<int>> board = { {0, 0, 0, 0, 0, 0, 0, 0},
                                            {0, 0, 1, 0, 1, 0, 1, 0},
                                            {0, 0, 0, 0, 0, 0, 0, 0},
                                            {0, 0, 1, 0, 1, 0, 1, 0},
                                            {0, 0, 0, 0, 0, 0, 0, 0},
                                            {0, 0, 1, 0, 1, 0, 1, 0},
                                            {0, 0, 0, 0, 0, 0, 0, -1},
                                            {0, 0, 0, 0, 0, 0, 0, 0}};*/

//    std::vector<std::vector<position>> positions;
//    BoardActions::available_jump_sequences({6, 7}, {}, board, 1, positions);
    Checkers checkers(board);
    checkers.bot_vs_bot();
    return 0;
}
