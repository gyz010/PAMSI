//
// Created by gyz on 03.06.24.
//

#include "../inc/Evaluation.h"

double Evaluation::evaluate_random() {
    std::random_device rand;
    return rand() % 100;

}

double Evaluation::evaluate_11params(const std::vector<std::vector<int>> &board, bool is_black) {
    double eval = 0;

    int number_of_enemy_pieces = 0;

    int number_of_pawns = 0;
    int number_of_kings = 0;
    int total_distance_to_promotion = 0;
    int save_pawn = 0;
    int save_king = 0;
    int defending_piece = 0;
    int attacking_pawn = 0;
    int central_pawn = 0;
    int central_king = 0;
    int movable_pawn = 0;
    int movable_king = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (is_friendly_pawn(board[row][col], is_black)) {
                number_of_pawns++;
                total_distance_to_promotion += distance_to_promotion(row, is_black);
                if (is_save(col)) save_pawn++;
                if (is_defending(row, is_black)) defending_piece++;
                if (is_attacking_pawn(row, is_black)) attacking_pawn++;
                if (is_central(row)) central_pawn++;
                if (is_movable(board, {row, col}, is_black)) movable_pawn++;
            } else if (is_friendly_king(board[row][col], is_black)) {
                number_of_kings++;
                if (is_save(col)) save_king++;
                if (is_defending(row, is_black)) defending_piece++;
                if (is_central(row)) central_king++;
                if (is_movable(board, {row, col}, is_black)) movable_king++;
            } else {
                number_of_enemy_pieces++;
            }
        }
    }
    eval = 1 * number_of_pawns +
           1.9 * number_of_kings +
           -0.0625 * total_distance_to_promotion +
           0.3 * save_pawn +
           0.6 * save_king +
           0.65 * defending_piece +
           0.9 * attacking_pawn +
           0.5 * central_pawn +
           1 * central_king +
           0.5 * movable_pawn +
           1 * movable_king;

    //Instynkt zabójcy
    if(number_of_enemy_pieces == 0) eval += 1000;
    if(number_of_enemy_pieces<3) {
        eval += 2.5 * (3-number_of_enemy_pieces);
    }
    return eval;
}

double Evaluation::evaluate_11(const std::vector<std::vector<int>> &board, bool is_black) {
    return (evaluate_11params(board, is_black) - evaluate_11params(board, !is_black));

}
double Evaluation::evaluate_25params(const std::vector<std::vector<int>> &board, bool is_black, const std::vector<double> &parameters) {
    double eval=0;
    //* PARAMETERS *//
    int number_of_pawns=0;
    int number_of_kings=0;
    int save_pawn=0;
    int save_king=0;
    int movable_pawn=0;
    int movable_king=0;
    int total_distance_to_promotion=0;
    int empty_promotion_fields=0;
    int defending_piece=0;
    int attacking_pawn=0;
    int central_pawn=0;
    int central_king=0;
    int main_diagonal_pawn=0;
    int main_diagonal_king=0;
    int double_diagonal_pawn=0;
    int double_diagonal_king=0;
    int loner_pawn=0;
    int loner_king=0;
    int holes=0;
    bool is_triangle_pattern = triangle_pattern(board, is_black);
    bool is_oreo_pattern = oreo_pattern(board, is_black);
    bool is_bridge_pattern = bridge_pattern(board, is_black);
    bool is_dog_pattern = dog_pattern(board, is_black);
    bool is_corner_pawn = pawn_in_corner(board, is_black);
    bool is_corner_king = king_in_corner(board, is_black);
    for(int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 0) continue; // pola niedostępne
            empty_promotion_fields += is_empty_promotion_field(board[row][col], row, is_black);
            holes += is_hole(board, row, col, is_black);
            if(is_friendly_pawn(board[row][col], is_black)) {
                number_of_pawns++;
                save_pawn += is_save(col);
                movable_pawn += is_movable(board, {row, col}, is_black);
                total_distance_to_promotion += distance_to_promotion(row, is_black);
                defending_piece += is_defending(row, is_black);
                attacking_pawn += is_attacking_pawn(row, is_black);
                central_pawn += is_central(row);
                main_diagonal_pawn += is_on_main_diagonal(row, col);
                double_diagonal_pawn += is_on_double_diagonal(row, col);
                loner_pawn += is_loner(board, row, col);
            }
            if(is_friendly_king(board[row][col], is_black)) {
                number_of_kings++;
                save_king += is_save(col);
                movable_king += is_movable(board, {row, col}, is_black);
                central_king += is_central(row);
                main_diagonal_king += is_on_main_diagonal(row, col);
                double_diagonal_king += is_on_double_diagonal(row, col);
                loner_king += is_loner(board, row, col);
            }
        }
    }
    eval = number_of_pawns * parameters[0] +
              number_of_kings * parameters[1] +
              total_distance_to_promotion * parameters[2] +
              save_pawn * parameters[3] +
              save_king * parameters[4] +
              defending_piece * parameters[5] +
              attacking_pawn * parameters[6] +
              central_pawn * parameters[7] +
              central_king * parameters[8] +
              movable_pawn * parameters[9] +
              movable_king * parameters[10] +
              empty_promotion_fields * parameters[11] +
              main_diagonal_pawn * parameters[12] +
              main_diagonal_king * parameters[13] +
              double_diagonal_pawn * parameters[14] +
              double_diagonal_king * parameters[15] +
              loner_pawn * parameters[16] +
              loner_king * parameters[17] +
              holes * parameters[18] +
              is_triangle_pattern * parameters[19] +
              is_oreo_pattern * parameters[20] +
              is_bridge_pattern * parameters[21] +
              is_dog_pattern * parameters[22] +
              is_corner_pawn * parameters[23] +
              is_corner_king * parameters[24];

    return eval;
}


double Evaluation::evaluate_25(const std::vector<std::vector<int>> &board, bool is_black,
                               const std::vector<double> &parameters) {
    return (evaluate_25params(board, is_black, parameters) - evaluate_25params(board, !is_black, parameters));

}

    bool Evaluation::is_friendly(int piece, bool is_black) {
        if(is_black and piece>0) return true;
        if(!is_black and piece<0) return true;
        return false;
    }

    bool Evaluation::is_friendly_pawn(int piece, bool is_black) {
        if(is_friendly(piece, is_black) and std::abs(piece) == 1) return true;
        return false;
    }

    bool Evaluation::is_friendly_king(int piece, bool is_black) {
        if(is_friendly(piece, is_black) and std::abs(piece) == 2) return true;
        return false;
    }

    int Evaluation::distance_to_promotion(int row, bool is_black) {
        if(is_black) return 7-row;
        return row;
    }


    bool Evaluation::is_save(int col) {
        if(col == 0 or col == 7) return true;
        return false;
    }

    bool Evaluation::is_movable(const std::vector<std::vector<int>> &board, position pos, bool is_black) {
        if(is_black) {
            if(pos.row+1 < 8 and pos.col+1 < 8 and board[pos.row+1][pos.col+1] == 0) return true;
            if(pos.row+1 < 8 and pos.col-1 >= 0 and board[pos.row+1][pos.col-1] == 0) return true;
        } else {
            if(pos.row-1 >= 0 and pos.col+1 < 8 and board[pos.row-1][pos.col+1] == 0) return true;
            if(pos.row-1 >= 0 and pos.col-1 >= 0 and board[pos.row-1][pos.col-1] == 0) return true;
        }
        return false;
    }

    bool Evaluation::is_defending(int row, bool is_black) {
        if(is_black and row < 2) return true;
        if(!is_black and row > 5) return true;
        return false;
    }

    bool Evaluation::is_attacking_pawn(int row, bool is_black) {
        if(is_black and row > 4) return true;
        if(!is_black and row < 3) return true;
        return false;
    }

    bool Evaluation::is_central(int row) {
        if((row == 3 or row == 4)) return true;
        return false;
    }

double Evaluation::evaluate_simple(const std::vector<std::vector<int>> &board, bool is_black) {
    double eval=0;
    int side = is_black ? 1 : -1;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            eval += board[row][col] * side;
        }
    }
    return eval;
}

bool Evaluation::is_empty_promotion_field(int piece, int row, bool is_black) {
    if(piece == 0 and ((is_black and row == 0) or (!is_black and row == 7))) return true;
    return false;
}

bool Evaluation::is_on_main_diagonal(int row, int col) {
    if(row == col) return true;
    return false;
}

bool Evaluation::is_on_double_diagonal(int row, int col) {
    if((row+col == 5) or (row+col == 9)) return true;
    return false;
}

bool Evaluation::is_loner(const std::vector<std::vector<int>> &board, int row, int col) {
    for(int r = row-1; r <= row+1; r+=2) {
        for(int c = col-1; c <= col+1; c+=2) {
            if(r < 0 or r >= 8 or c < 0 or c >= 8) continue;
            if(board[r][c] != 0) return false;
        }
    }
    return true;
}

bool Evaluation::is_hole(const std::vector<std::vector<int>> &board, int row, int col, bool is_black) {
    int adjacent_pieces = 0;
    int side = is_black ? 1 : -1;
    if(row == 0 or row == 7 or col == 0 or col == 7) return false;
    for(int r = row-1; r <= row+1; r+=2) {
        for(int c = col-1; c <= col+1; c+=2) {
            if(board[r][c] * side>0) adjacent_pieces++;
        }
    }
    if(adjacent_pieces == 3) return true;
    return false;
}

bool Evaluation::triangle_pattern(const std::vector<std::vector<int>> &board, bool is_black) {
    constexpr position black_triangle[3] = {{0, 1}, {0, 3}, {1, 2}};
    constexpr position white_triangle[3] = {{7, 4}, {7, 6}, {6, 5}};
    if(is_black) {
        for(auto &pos: black_triangle) {
            if(board[pos.row][pos.col] != 1) return false;
        }
        return false;
    }
    else {
        for(auto &pos: white_triangle) {
            if(board[pos.row][pos.col] != -1) return false;
        }
        return true;
    }
}

bool Evaluation::oreo_pattern(const std::vector<std::vector<int>> &board, bool is_black) {
    constexpr position black_oreo[3] = {{0, 3}, {0, 5}, {1, 4}};
    constexpr position white_oreo[3] = {{7, 2}, {7, 4}, {6, 3}};
    if(is_black) {
        for(auto &pos: black_oreo) {
            if(board[pos.row][pos.col] != 1) return false;
        }
        return false;
    }
    else {
        for(auto &pos: white_oreo) {
            if(board[pos.row][pos.col] != -1) return false;
        }
        return true;
    }
}

bool Evaluation::bridge_pattern(const std::vector<std::vector<int>> &board, bool is_black) {
    constexpr position black_bridge[2] = {{0, 1}, {0, 5}};
    constexpr position white_bridge[2] = {{7, 2}, {7, 6}};
    if(is_black) {
        for(auto &pos: black_bridge) {
            if(board[pos.row][pos.col] != 1) return false;
        }
        return false;
    }
    else {
        for(auto &pos: white_bridge) {
            if(board[pos.row][pos.col] != -1) return false;
        }
        return true;
    }
}

bool Evaluation::dog_pattern(const std::vector<std::vector<int>> &board, bool is_black) {
    constexpr position black_dog[2] = {{0, 1}, {1, 0}};
    constexpr position white_dog[2] = {{7, 6}, {6, 7}};
    if(is_black) {
        if(board[black_dog[0].row][black_dog[0].col] == 1 and board[black_dog[1].row][black_dog[1].col] < 0)
            return true;
    }
    else {
        if(board[white_dog[0].row][white_dog[0].col] == -1 and board[white_dog[1].row][white_dog[1].col] > 0)
            return true;
    }
    return false;
}

bool Evaluation::pawn_in_corner(const std::vector<std::vector<int>> &board, bool is_black) {
    if(is_black) {
        if(board[0][7] == 1) return true;
    }
    else {
        if(board[7][0] == -1) return true;
    }
    return false;
}

bool Evaluation::king_in_corner(const std::vector<std::vector<int>> &board, bool is_black) {
    if(is_black) {
        if(board[7][0] == 2) return true;
    }
    else {
        if(board[0][7] == -2) return true;
    }
    return false;
}




