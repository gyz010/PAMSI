#include <iostream>
#include "../inc/Checkers.h"
#include "../inc/Bot.h"
int main() {
    enum interface {NET, GUI};
    int64_t seed = 0;
    bool is_black = true;
    int depth = 7;
    interface game_type = GUI;



    std::vector<std::vector<int>> board = { { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 1, 0,  1, 0,  1, 0,  1, 0},
                                            { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            {-1, 0, -1, 0, -1, 0, -1, 0},
                                            { 0,-1,  0,-1,  0,-1,  0,-1},
                                            {-1, 0, -1, 0, -1, 0, -1, 0} };

    Checkers checkers(board);
    Bot bot(is_black, depth, seed);
    bot.fillTree(bot.root, 0, board, 0);
    bot.minmax(bot.root,
               0, true,
               std::numeric_limits<double>::min(),
               std::numeric_limits<double>::max(),
               board);
    checkers.bot_vs_bot();
    return 0;
}
