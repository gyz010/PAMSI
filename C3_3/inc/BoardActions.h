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
#include <algorithm>


struct position {
    int row, col;
    bool operator==(const position &other) const {
        return row == other.row && col == other.col;
    }
    bool operator<(const position &other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
    position operator-(const position &other) const {
        return {row - other.row, col - other.col};
    }
};

class BoardActions {
    static std::unordered_map<std::string, position> notation_to_position_map;

public:

    static bool is_king(const position &pos, std::vector<std::vector<int>> &board);

    static bool is_valid_square(const position &pos);

    static bool is_legal_move(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn);

    static bool move(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn);

    static bool jump(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn);

    static bool action(const std::string &notation, std::vector<std::vector<int>> &board, int turn, int &no_jumps_count);

    static std::vector<position> available_moves(std::vector<std::vector<int>> &board, int turn);

    static std::vector<position> available_jumps(std::vector<std::vector<int>> &board, int turn);

    static std::vector<position> available_jumps(std::vector<std::vector<int>> &board, int turn, std::vector<position> &froms);


    static std::vector<position> convert_notation(const std::string &notation);

    static std::vector<position> available_moves_from(const position &from,
                                                      std::vector<std::vector<int>> &board,
                                                      int turn);


    static std::vector<position> available_jumps_from(const position &from,
                                                      std::vector<std::vector<int>> &board,
                                                      int turn);

    static void available_jump_sequences(const position &from,
                                         std::vector<position> current_sequence,
                                         std::vector<std::vector<int>> temp_board,
                                         int turn,
                                         std::vector<std::vector<position>> &sequences);

    static std::string position_to_notation(std::vector<position> &pos);


    static void no_legal_check_jump(const position &from,
                                    const position &to,
                                    std::vector<std::vector<int>> &board,
                                    std::vector<int> &taken_pieces,
                                    bool &promoted);

    static void no_legal_check_move(const position &from,
                                    const position &to,
                                    std::vector<std::vector<int>> &board,
                                    bool &promoted);

    static void no_legal_check_action(const std::string &notation,
                                      std::vector<std::vector<int>> &board,
                                      std::vector<int> &taken_pieces,
                                      bool &promoted);

    static void undo_move(const position &from,
                          const position &to,
                          std::vector<std::vector<int>> &board,
                          bool promoted);

    static void undo_jump(const std::vector<position> &jumps,
                            std::vector<std::vector<int>> &board,
                            std::vector<int> &taken_pieces,
                            bool promoted);

    static void undo_action(const std::string &notation,
                           std::vector<std::vector<int>> &board,
                           std::vector<int> &taken_pieces,
                           bool promoted);
};


#endif //C3_3_BOARDACTIONS_H
