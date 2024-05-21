#include <iostream>
#include "../inc/Checkers.h"
#include "../inc/Bot.h"
int main() {

    std::vector<std::vector<int>> board = { { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 1, 0,  1, 0,  1, 0,  1, 0},
                                            { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            {-1, 0, -1, 0, -1, 0, -1, 0},
                                            { 0,-1,  0,-1,  0,-1,  0,-1},
                                            {-1, 0, -1, 0, -1, 0, -1, 0} };

    Checkers checkers(board);

//    Bot bot(3, board);
    checkers.play();
    return 0;
}
