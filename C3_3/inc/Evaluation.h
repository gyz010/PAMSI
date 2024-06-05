//
// Created by gyz on 03.06.24.
//

#ifndef C3_3_EVALUATION_H
#define C3_3_EVALUATION_H

#include <vector>
#include <random>
#include "BoardActions.h"
class Evaluation {
private:
    /** Figury gracza **/
    static bool is_friendly(int piece, bool is_black);
    static bool is_friendly_pawn(int piece, bool is_black);
    static bool is_friendly_king(int piece, bool is_black);

    /** Odledłość od promocji **/
    static int distance_to_promotion(int row, bool is_black);

    /** Na horyzontalnej krawędzi planszy **/
    static bool is_save(int col);

    /** Zdolne do ruchu **/
    static bool is_movable(const std::vector<std::vector<int>> &board, position pos, bool is_black);

    /** Pozycja **/
    static bool is_defending(int row, bool is_black);
    static bool is_attacking_pawn(int row, bool is_black);
    static bool is_central(int row);
    static bool is_empty_promotion_field(int piece, int row, bool is_black);
    static bool is_on_main_diagonal(int row, int col);
    static bool is_on_double_diagonal(int row, int col);
    static bool is_loner(const std::vector<std::vector<int>> &board, int row, int col);
    static bool is_hole(const std::vector<std::vector<int>> &board, int row, int col, bool is_black);
    static bool triangle_pattern(const std::vector<std::vector<int>> &board, bool is_black);
    static bool oreo_pattern(const std::vector<std::vector<int>> &board, bool is_black);
    static bool bridge_pattern(const std::vector<std::vector<int>> &board, bool is_black);
    static bool dog_pattern(const std::vector<std::vector<int>> &board, bool is_black);
    static bool pawn_in_corner(const std::vector<std::vector<int>> &board, bool is_black);
    static bool king_in_corner(const std::vector<std::vector<int>> &board, bool is_black);


    static double evaluate_11params(const std::vector<std::vector<int>> &board, bool is_black);
    static double evaluate_25params(const std::vector<std::vector<int>> &board, bool is_black, const std::vector<double> &parameters);
public:
    static double evaluate_random();
    static double evaluate_simple(const std::vector<std::vector<int>> &board, bool is_black);
    static double evaluate_11(const std::vector<std::vector<int>> &board, bool is_black);
    static double evaluate_25(const std::vector<std::vector<int>> &board, bool is_black, const std::vector<double> &parameters);
};


#endif //C3_3_EVALUATION_H
