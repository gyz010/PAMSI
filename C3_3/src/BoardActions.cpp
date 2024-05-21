//
// Created by gyz on 21.05.24.
//

#include "../inc/BoardActions.h"

std::map<std::string, position> notation_map;

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

    //Zły kolor pionka
    if(turn%2 == board[from.row][from.col] > 0) return false;

    //Nie może być dostępnych bić
    if(!available_jumps(board, turn).empty()) return false;

    //Pole 'from' musi być zajęte
    if (board[from.row][from.col] == 0) return false;

    //Pole 'to' musi być puste
    if (board[to.row][to.col] != 0) return false;

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
            std::vector<position> jumps_from = available_jumps_from(pos, board[row][col], board, turn);
            jumps.insert(jumps.end(), jumps_from.begin(), jumps_from.end());
        }
    }
    return jumps;
}

std::vector<position> BoardActions::available_jumps_from(const position &from, const int piece,
                                                         std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> jumps;
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
    } else { // Pion
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

std::vector<position> BoardActions::available_moves(std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> moves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (turn % 2 == board[row][col] > 0) continue;
            std::vector<position> moves_from = available_moves_from({row, col}, board[row][col], board, turn);
            moves.insert(moves.end(), moves_from.begin(), moves_from.end());
        }
    }
    return moves;
}

std::vector<position> BoardActions::available_moves_from(const position &from, int piece, std::vector<std::vector<int>> &board, int turn) {
    std::vector<position> moves;
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

bool BoardActions::jump(const position &from, const position &to, std::vector<std::vector<int>> &board, int turn) {
    //Sprawdzenie czy bije poprawny kolor
    if (turn % 2 == board[from.row][from.col] > 0) return false;

    std::vector<position> possible_jumps = available_jumps_from(from, board[from.row][from.col], board, turn);
    for(const position &jump : possible_jumps) {
        if(jump == to) {
            board[to.row][to.col] = board[from.row][from.col];
            board[from.row][from.col] = 0;

            //Usunięcie zbitego pionka
            position middle = {(from.row + to.row) / 2, (from.col + to.col) / 2};
            board[middle.row][middle.col] = 0;

            //Promocja
            if ((to.row == 0 || to.row == 7) && !is_king(to, board)) {
                board[to.row][to.col] *= 2;
                turn++;
            }
            return true;
        }
    }
    return false;
}

bool BoardActions::action(const std::string &notation, std::vector<std::vector<int>> &board, int turn) {
    //Notacja ruchu w postaci "from-to", from, to = (1, 32)
    //Notacja bicia w postaci "fromxto1xto2"
    std::vector<position> moves;
    //Bicie
    if(notation.find('-') == std::string::npos) {
        moves = convert_notation(notation);
        if (moves.size() < 2) return false;
        std::vector<std::vector<int>> board_backup = board;
        for (int i=0; i<moves.size()-1; i++) {
            if (!jump(moves[i], moves[i+1], board, turn))  {
                board = board_backup;
                return false;
            }
            // W biciu doszło do promocji więc zakończ ruch
            if (turn % 2 == board[moves[i+1].row][moves[i+1].col] > 0) {
                if (i+2 == moves.size()) return true;
                    // Nielegalna próba bicia po promocji
                else {
                    board = board_backup;
                    turn--;
                    return false;
                }
            }
            // Sprawdzenie czy nie zostało więcej możliwych bić
            if (i==moves.size()-2 && !available_jumps(board, turn).empty()) {
                board = board_backup;
                return false;
            }

        }
        //Zakończenie bicia
        turn++;
    }
        //Ruch
    else if(notation.find('x') == std::string::npos) {
        moves = convert_notation(notation);
        if (moves.size() != 2) return false;
        if (!move(moves[0], moves[1], board, turn)) return false;
    }
    return true;
}

std::vector<position> BoardActions::convert_notation(const std::string &notation) {
    if(notation_map.empty()) {
        int count = 1;
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                if ((row + col) % 2 == 1) {
                    notation_map[std::to_string(count)] = {row, col};
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
            if(token[0] >= 'A' && token[0] <= 'H') {
                moves.push_back({token[1] - '1', token[0] - 'A'});
            } else {
                moves.push_back(notation_map[token]);
            }
        }
    }
    if (notation.find('x') == std::string::npos) {
        while (std::getline(ss, token, '-')) {
            if(token[0] >= 'A' && token[0] <= 'H') {
                moves.push_back({token[1] - '1', token[0] - 'A'});
            } else {
                moves.push_back(notation_map[token]);
            }
        }
    }
    return moves;
}