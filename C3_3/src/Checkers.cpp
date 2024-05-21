#include "../inc/Checkers.h"


Checkers::Checkers(std::vector<std::vector<int>> &board) : board(board){
    turn = 0;
    no_jumps_count = 0;
    result =  NONE;


}

void Checkers::draw_board() {
    std::cout << "Turn " << turn << std::endl;
    std::cout << "  A B C D E F G H" << std::endl; // column numeration
    for (int row = 0; row < 8; ++row) {
        std::cout << row + 1 << " "; // row numeration
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == 0) {
                if((row+col)%2 == 0) std::cout << "  ";
                else std::cout << "* ";
            } else if (std::abs(board[row][col]) == 1) {
                std::cout << (board[row][col] > 0 ? "\u26C0" : "\u26C2") << ' ';
            } else {
                std::cout << (board[row][col] > 0 ? "\u26C1" : "\u26C3") << ' ';
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Enter move: ";
}

bool Checkers::is_king(const position &pos) {
    return std::abs(board[pos.row][pos.col]) == 2;
}

bool Checkers::is_valid_square(const position &pos) {
    if (pos.row < 0 || pos.row >= 8 || pos.col < 0 || pos.col >= 8) return false; //Poza planszą
    if ((pos.row + pos.col)%2 == 0) return false; //Białe pola
    return true;
}

bool Checkers::is_legal_move(const position &from, const position &to) {
    //Ruch musi być w obrębie planszy
    if((!is_valid_square(from)) || (!is_valid_square(to))) return false;

    //Zły kolor pionka
    if(turn%2 == board[from.row][from.col] > 0) return false;

    //Nie może być dostępnych bić
    if(!available_jumps().empty()) return false;

    //Pole 'from' musi być zajęte
    if (board[from.row][from.col] == 0) return false;

    //Pole 'to' musi być puste
    if (board[to.row][to.col] != 0) return false;

    //Figura może poruszać się tylko jedno pole na skos
    if (std::abs(to.row - from.row) != 1 || std::abs(to.col - from.col) != 1) return false;

    //Zasady dla piona:
    if(!is_king(from)){
        //Pion nie może poruszać się wstecz
        if ((to.row - from.row) != board[from.row][from.col]) return false;

    }
    return true;
}

std::vector<position> Checkers::available_jumps() {
    std::vector<position> jumps;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (turn % 2 == board[row][col] > 0) continue;
            position pos = {row, col};
            std::vector<position> jumps_from = available_jumps_from(pos, board[row][col]);
            jumps.insert(jumps.end(), jumps_from.begin(), jumps_from.end());
        }
    }
    return jumps;
}

std::vector<position> Checkers::available_jumps_from(const position &from, const int piece) {
    std::vector<position> jumps;
    if (is_king(from)) { // Król
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

bool Checkers::move(const position &from, const position &to) {
    if (!is_legal_move(from, to)) {
        std::cout << "Illegal move" << std::endl;
        return false;
    }
    board[to.row][to.col] = board[from.row][from.col];
    board[from.row][from.col] = 0;

    //Promocja
    if ((to.row == 0 || to.row == 7) && !is_king(to)) {
        board[to.row][to.col] *= 2;
    }
    turn++;
    no_jumps_count++;
    return true;
}

std::vector<position> Checkers::available_moves() {
    std::vector<position> moves;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 0) continue;
            if (turn % 2 == board[row][col] > 0) continue;
            std::vector<position> moves_from = available_moves_from({row, col}, board[row][col]);
            moves.insert(moves.end(), moves_from.begin(), moves_from.end());
        }
    }
    return moves;
}

std::vector<position> Checkers::available_moves_from(const position &from, int piece) {
    std::vector<position> moves;
    if (is_king(from)) {
        for (int row = -1; row <= 1; row += 2) {
            for (int col = -1; col <= 1; col += 2) {
                position to = {from.row + row, from.col + col};
                if(is_legal_move(from, to)) {
                    moves.push_back(to);
                }
            }
        }
    } else {
        for (int col = -1; col <= 1; col += 2) {
            position to = {from.row + piece, from.col + col};
            if (is_legal_move(from, to)) {
                moves.push_back(to);
            }
        }
    }
    return moves;
}

bool Checkers::jump(const position &from, const position &to) {
    //Sprawdzenie czy bije poprawny kolor
    if (turn % 2 == board[from.row][from.col] > 0) return false;

    std::vector<position> possible_jumps = available_jumps_from(from, board[from.row][from.col]);
    for(const position &jump : possible_jumps) {
        if(jump == to) {
            board[to.row][to.col] = board[from.row][from.col];
            board[from.row][from.col] = 0;

            //Usunięcie zbitego pionka
            position middle = {(from.row + to.row) / 2, (from.col + to.col) / 2};
            board[middle.row][middle.col] = 0;

            //Promocja
            if ((to.row == 0 || to.row == 7) && !is_king(to)) {
                board[to.row][to.col] *= 2;
                turn++;
            }
            no_jumps_count=0;
            return true;
        }
    }
    return false;
}

bool Checkers::action(const std::string &notation) {
    //Notacja ruchu w postaci "from-to", from, to = (1, 32)
    //Notacja bicia w postaci "fromxto1xto2"
    std::vector<position> moves;
    //Bicie
    if(notation.find('-') == std::string::npos) {
        moves = convert_notation(notation);
        if (moves.size() < 2) return false;
        std::vector<std::vector<int>> board_backup = board;
        for (int i=0; i<moves.size()-1; i++) {
            if (!jump(moves[i], moves[i+1]))  {
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
            if (i==moves.size()-2 && !available_jumps().empty()) {
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
        if (!move(moves[0], moves[1])) return false;
    }
    return true;
}

void Checkers::play() {
    std::string notation;
    while (true) {
        draw_board();
        std::cin >> notation;
        int temp = turn;
        if (!action(notation)) {
            result = (temp%2==0) ? WHITE_WIN : BLACK_WIN;
            break;
        }
        if(checkWinCondition()) break;
    }
    switch (result) {
        case BLACK_WIN:
            std::cout << "Black wins" << std::endl;
            break;
        case WHITE_WIN:
            std::cout << "White wins" << std::endl;
            break;
        case DRAW:
            std::cout << "Draw" << std::endl;
        case NONE:
            //Nie powinno się zdarzyć
            std::cout << "???" << std::endl;
            break;
    }
}

std::vector<position> Checkers::convert_notation(const std::string &notation) {
    std::vector<position> moves;
    std::stringstream ss(notation);
    std::string token;
    if (notation.find('-') == std::string::npos) {
        while (std::getline(ss, token, 'x')) {
            if(token[0] >= 'A' && token[0] <= 'H') {
                moves.push_back({token[1] - '1', token[0] - 'A'});
            } else {
                moves.push_back(BoardActions::notation_map[token]);
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

bool Checkers::checkWinCondition() {
    int black_count = 0;
    int white_count = 0;

    if(no_jumps_count==40) {
        result = DRAW;
        return true;
    }

    for(auto &row : board) {
        for(auto &field : row) {
            if(field>0) black_count++;
            if(field<0) white_count++;
        }

    }
    if(black_count==0) result = WHITE_WIN;
    else if(white_count==0) result = BLACK_WIN;

    if(result!=NONE) return true;
    return false;
}

// Getters
int Checkers::getTurn() const {
    return turn;
}

std::vector<std::vector<position>> Checkers::available_jumps_sequence(const position &from, int piece) {
    std::vector<std::vector<int>> temp_board = board;
    std::vector<std::vector<position>> sequences;
    std::vector<position> jumps = available_jumps_from(from, piece);
    for(const position &jump : jumps) {
        std::vector<position> seq = {from, jump};
        available_jumps_seq_recursion(jump, seq, sequences);
    }
    return sequences;
}

void Checkers::available_jumps_seq_recursion(const position &from, std::vector<position> &seq,
                                             std::vector<std::vector<position>> &result) {
    std::vector<position> jumps = available_jumps_from(from, board[from.row][from.col]);
    if(jumps.empty()) {
        result.push_back(seq);
        return;
    }
    for(const position &jump : jumps) {
        seq.push_back(jump);
        available_jumps_seq_recursion(jump, seq, result);
        seq.pop_back();
    }
}


