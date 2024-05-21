//
// Created by gyz on 20.05.24.
//

#include "../inc/Bot.h"

void Bot::fillTree(Node &node, int depth, std::vector<std::vector<int>> temp_board, bool black_to_move) {
    if(depth == this->depth) {
        return;
    }
    std::string move;
    for(int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if(temp_board[row][col] == 0) continue;
            if((temp_board[row][col] > 0) != black_to_move) continue;

            auto jumps = available_jumps_sequence({row, col}, temp_board[row][col]);
            for(auto &jump : jumps) {
                for (auto &to: jump) {

                }
            }
        }
    }
}