//
// Created by gyz on 21.05.24.
//

#include <chrono>
#include "../inc/BoardActions.h"

std::unordered_map <std::string, position> BoardActions::notation_to_position_map;

bool BoardActions::is_king(const position &pos, std::vector<std::vector<int>> &board) {
    return std::abs(board[pos.row][pos.col]) == 2;
}

bool BoardActions::is_valid_square(const position &pos) {
    if (pos.row < 0 || pos.row >= 8 || pos.col < 0 || pos.col >= 8) return false; //Poza planszą
    if ((pos.row + pos.col)%2 == 0) return false; //Białe pola
    return true;
}

bool BoardActions::is_legal_move(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn) {
    //Ruch musi być w obrębie planszy
    if((!is_valid_square(from)) || (!is_valid_square(to))) return false;

    //Pole 'to' musi być puste
    if (board[to.row][to.col] != 0) return false;

    //Pole 'from' musi być zajęte
    if (board[from.row][from.col] == 0) return false;

    //Zły kolor pionka
    if(turn%2 == board[from.row][from.col] > 0) return false;

    //Nie może być dostępnych bić
//    if(!available_jumps(board, turn).empty()) return false;


    //Figura może poruszać się tylko jedno pole na skos
    if (std::abs(to.row - from.row) != 1 || std::abs(to.col - from.col) != 1) return false;

    //Zasady dla piona:
    if(!is_king(from, board)){
        //Pion nie może poruszać się wstecz
        if ((to.row - from.row) != board[from.row][from.col]) return false;

    }
    return true;
}

std::vector<position> BoardActions::available_jumps(std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> jumps;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (turn % 2 == board[row][col] > 0) continue;
            position pos = {row, col};
            std::vector<position> jumps_from = available_jumps_from(pos, board, turn);
            jumps.insert(jumps.end(), jumps_from.begin(), jumps_from.end());
        }
    }
    return jumps;
}

std::vector<position> BoardActions::available_jumps(std::vector<std::vector<int>> &board, int turn, std::vector<position> &froms) {

    std::vector<position> jumps;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (turn % 2 == board[row][col] > 0) continue;
            position pos = {row, col};
            std::vector<position> jumps_from = available_jumps_from(pos, board, turn);
            jumps.insert(jumps.end(), jumps_from.begin(), jumps_from.end());
            if(!jumps_from.empty()) froms.push_back(pos);
        }
    }
    return jumps;
}

std::vector<position> BoardActions::available_jumps_from(const position &from,
                                                         std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> jumps;
    int piece = board[from.row][from.col];
    if (is_king(from, board)) { // Król
        for (int row = -1; row <= 1; row += 2) {
            for (int col = -1; col <= 1; col += 2) {
                if (!is_valid_square({from.row + row, from.col + col})) continue;
                if (board[from.row + row][from.col + col] * piece < 0) { //Jeśli na polu stoi przeciwnik
                    if (!is_valid_square({from.row + 2 * row, from.col + 2 * col})) continue;
                    if (board[from.row + 2 * row][from.col + 2 * col] == 0) { //Jeśli pole za przeciwnikiem jest puste
                        jumps.push_back({from.row + 2 * row, from.col + 2 * col});
                    }
                }
            }
        }
    }
    else { // Pion
        int row = piece;
        for (int col = -1; col <= 1; col += 2) {
            if (!is_valid_square({from.row + row, from.col + col})) continue;
            if (board[from.row + row][from.col + col] * piece < 0) { //Jeśli na polu stoi przeciwnik
                if (!is_valid_square({from.row + 2 * row, from.col + 2 * col})) continue;
                if (board[from.row + 2 * row][from.col + 2 * col] == 0) { //Jeśli pole za przeciwnikiem jest puste
                    jumps.push_back({from.row + 2 * row, from.col + 2 * col});
                }
            }
        }
    }
    return jumps;
}

std::vector<position> BoardActions::available_moves(std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> moves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (turn % 2 == board[row][col] > 0) continue;
            std::vector<position> moves_from = available_moves_from({row, col}, board, turn);
            moves.insert(moves.end(), moves_from.begin(), moves_from.end());
        }
    }
    return moves;
}

std::vector<position> BoardActions::available_moves_from(const position &from, std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> moves;
    int piece = board[from.row][from.col];
    if (is_king(from, board)) {
        for (int row = -1; row <= 1; row += 2) {
            for (int col = -1; col <= 1; col += 2) {
                position to = {from.row + row, from.col + col};
                if(is_legal_move(from, to, board, turn)) {
                    moves.push_back(to);
                }
            }
        }
    } else {
        for (int col = -1; col <= 1; col += 2) {
            position to = {from.row + piece, from.col + col};
            if (is_legal_move(from, to, board, turn)) {
                moves.push_back(to);
            }
        }
    }
    return moves;
}

bool BoardActions::move(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn) {
    if (!is_legal_move(from, to, board, turn)) {
        std::cout << "Illegal move" << std::endl;
        return false;
    }
    board[to.row][to.col] = board[from.row][from.col];
    board[from.row][from.col] = 0;

    //Promocja
    if ((to.row == 0 || to.row == 7) && !is_king(to, board)) {
        board[to.row][to.col] *= 2;
    }
    turn++;
    return true;
}

bool BoardActions::jump(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn) {
    //Sprawdzenie czy bije poprawny kolor
    if (turn % 2 == board[from.row][from.col] > 0) return false;

    std::vector<position> possible_jumps = available_jumps_from(from, board, turn);
    for(const position &jump : possible_jumps) {
        if(jump == to) {
            board[to.row][to.col] = board[from.row][from.col];
            board[from.row][from.col] = 0;

            //Usunięcie zbitego pionka
            position middle = {(from.row + to.row) / 2, (from.col + to.col) / 2};
            board[middle.row][middle.col] = 0;
            return true;
        }
    }
    return false;
}
bool BoardActions::action(const std::string &notation, std::vector<std::vector<int>> &board, int turn, int &no_jumps_count) {
    //Notacja ruchu w postaci "from-to", from, to = (1, 32)
    //Notacja bicia w postaci "fromxto1xto2"
    std::vector<position> moves;
    //Bicie
    if(notation.find('-') == std::string::npos) {
        no_jumps_count=0;
        moves = convert_notation(notation);
        if (moves.size() < 2) return false;
        std::vector<std::vector<int>> board_backup = board;
        for (int i=0; i<moves.size()-1; i++) {
            if (!jump(moves[i], moves[i+1], board, turn))  {
                board = board_backup;
                return false;
            }
            // W biciu doszło do promocji więc zakończ ruch
            if ((moves[i+1].row == 0 || moves[i+1].row == 7) && !is_king(moves[i+1], board)) {
                board[moves[i+1].row][moves[i+1].col] *= 2;
                turn++;
                if (i+2 == moves.size()) return true;
                    // Nielegalna próba bicia po promocji
                else {
                    board = board_backup;
                    turn--;
                    return false;
                }
            }
            // Sprawdzenie czy nie zostało więcej możliwych bić
            if (i==moves.size()-2 && !available_jumps_from(moves[i],
                                                           board,
                                                           turn).empty()) {
                board = board_backup;
                return false;
            }

        }
        //Zakończenie bicia
        turn++;
    }
        //Ruch
    else if(notation.find('x') == std::string::npos) {
        no_jumps_count++;
        moves = convert_notation(notation);
        if (std::abs(board[moves[0].row][moves[0].col]) == 1) no_jumps_count = 0;
        if (moves.size() != 2) return false;
        if (!move(moves[0], moves[1], board, turn)) return false;
    }
    else return false;
    return true;
}

std::vector<position> BoardActions::convert_notation(const std::string &notation) {
    if(notation_to_position_map.empty()) {
        int count = 1;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if ((row + col) % 2 == 1) {
                    notation_to_position_map[std::to_string(count)] = {row, col};
                    ++count;
                }
            }
        }
    }
    std::vector<position> moves;
    std::stringstream ss(notation);
    std::string token;
    if (notation.find('-') == std::string::npos) {
        while (std::getline(ss, token, 'x')) {
            moves.push_back({notation_to_position_map[token]});
        }
    }
    if (notation.find('x') == std::string::npos) {
        while (std::getline(ss, token, '-')) {
            moves.push_back(notation_to_position_map[token]);
        }
    }
    return moves;
}

void BoardActions::available_jump_sequences(const position &from, std::vector<position> current_sequence,
                                                std::vector<std::vector<int>> temp_board, int turn,
                                                std::vector<std::vector<position>> &sequences) {

    std::vector<position> jumps = available_jumps_from(from, temp_board, turn);
    if (jumps.empty()) {
        sequences.push_back(current_sequence);
        return;
    }
    for (const position &j : jumps) {
        std::vector<std::vector<int>> new_board = temp_board;
        jump(from, j, new_board, turn);
        std::vector<position> new_sequence = current_sequence;
        new_sequence.push_back(j);
        available_jump_sequences(j, new_sequence, new_board, turn, sequences);
    }
}

std::string BoardActions::position_to_notation(std::vector<position> &pos) {
    std::string notation;
    if(pos.size() < 2) return "";
    if(std::abs(pos[0].row - pos[1].row) == 1 && std::abs(pos[0].col - pos[1].col) == 1) { //Ruch
        for(const position &p : pos) {
            notation += std::to_string(p.row * 4 + p.col / 2 + 1);
            notation += '-';
        }
        notation.pop_back();
    } else { //Bicie
        for(const position &p : pos) {
            notation += std::to_string(p.row * 4 + p.col / 2 + 1);
            notation += 'x';
        }
        notation.pop_back();
    }
    return notation;
}

void BoardActions::no_legal_check_move(const position &from, const position &to, std::vector<std::vector<int>> &board,
                                       bool &promoted) {
    //Zmiana pozycji
    board[to.row][to.col] = board[from.row][from.col];
    board[from.row][from.col] = 0;

    //Promocja
    if ((to.row == 0 || to.row == 7) && !is_king(to, board)) {
        board[to.row][to.col] *= 2;
        promoted = true;
    }
}

void BoardActions::no_legal_check_jump(const position &from, const position &to, std::vector<std::vector<int>> &board,
                                       std::vector<int> &taken_pieces, bool &promoted) {
    //Zmiana pozycji
    board[to.row][to.col] = board[from.row][from.col];
    board[from.row][from.col] = 0;

    //Usunięcie zbitego pionka
    position middle = {(from.row + to.row) / 2, (from.col + to.col) / 2};
    taken_pieces.push_back(board[middle.row][middle.col]);
    board[middle.row][middle.col] = 0;

    //Promocja
    if ((to.row == 0 || to.row == 7) && !is_king(to, board)) {
        board[to.row][to.col] *= 2;
        promoted = true;
    }
}

void BoardActions::no_legal_check_action(const std::string &notation, std::vector<std::vector<int>> &board,
                                         std::vector<int> &taken_pieces, bool &promoted) {
    std::vector<position> moves;
    //Bicie
    if(notation.find('-') == std::string::npos) {
        moves = convert_notation(notation);
        for(int i=0; i<moves.size()-1; i++) {
            no_legal_check_jump(moves[i], moves[i+1], board, taken_pieces, promoted);
            // W biciu doszło do promocji więc zakończ ruch
            if(promoted) {
                return;
            }
        }
    }
    if(notation.find('x') == std::string::npos) {
        moves = convert_notation(notation);
        no_legal_check_move(moves[0], moves[1], board, promoted);
    }
}

void BoardActions::undo_move(const position &from, const position &to, std::vector<std::vector<int>> &board, bool promoted) {
    //Zmiana pozycji
    board[from.row][from.col] = board[to.row][to.col];
    board[to.row][to.col] = 0;

    //Democja
    if ((to.row == 0 || to.row == 7) && promoted && is_king(from, board)) {
        board[from.row][from.col] /= 2;
    }
}

void BoardActions::undo_jump(const std::vector<position> &jumps, std::vector<std::vector<int>> &board,
                             std::vector<int> &taken_pieces, bool promoted) {

    //Democja
    if((jumps.back().row == 0 || jumps.back().row == 7) && promoted && is_king(jumps.back(), board)) {
        board[jumps.back().row][jumps.back().col] /= 2;
    }
    for(int i=jumps.size()-2; i>=0; i--) {
        //Zmiana pozycji
        board[jumps[i].row][jumps[i].col] = board[jumps[i+1].row][jumps[i+1].col];
        board[jumps[i+1].row][jumps[i+1].col] = 0;

        //Odłożenie zbitego pionka
        position middle = {(jumps[i].row + jumps[i+1].row) / 2, (jumps[i].col + jumps[i+1].col) / 2};
        board[middle.row][middle.col] = taken_pieces.back();
        taken_pieces.pop_back();
    }
}

void BoardActions::undo_action(const std::string &notation, std::vector<std::vector<int>> &board, std::vector<int> &taken_pieces, bool promoted) {
    std::vector<position> moves;
    //Bicie
    if(!taken_pieces.empty()) {
        moves = convert_notation(notation);
        undo_jump(moves, board, taken_pieces, promoted);
    }
    //Ruch
    else {
        moves = convert_notation(notation);
        undo_move(moves[0], moves[1], board, promoted);
    }
}
