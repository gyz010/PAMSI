//
// Created by gyz on 21.05.24.
//

#ifndef C3_3_BOARDACTIONS_H
#define C3_3_BOARDACTIONS_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

struct position {
    int row, col;
    bool operator==(const position &other) const {
        return row == other.row && col == other.col;
    }
};

class BoardActions {
    static std::map<std::string, position> notation_map;
public:
    static bool is_king(const position &pos, std::vector<std::vector<int>> &board);
    static bool is_valid_square(const position &pos);
    static bool is_legal_move(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn);
    static bool move(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn);
    static bool jump(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn);
    static bool action(const std::string &notation, std::vector<std::vector<int>> &board, int turn);
    static std::vector<position> available_moves(std::vector<std::vector<int>> &board, int turn);
    static std::vector<position> available_jumps(std::vector<std::vector<int>> &board, int turn);
    static std::vector<position> convert_notation(const std::string &notation);
    static std::vector<position> available_moves_from(const position &from, int piece, std::vector<std::vector<int>> &board, int turn);
    static std::vector<position> available_jumps_from(const position &from, int piece, std::vector<std::vector<int>> &board, int turn);
    static std::vector<std::vector<position>> available_jumps_sequence(const position &from, int piece,
                                                                       std::vector<std::vector<int>> &board, int turn);
};


#endif //C3_3_BOARDACTIONS_H
