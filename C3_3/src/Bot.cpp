//
// Created by gyz on 20.05.24.
//

#include "../inc/Bot.h"

void Bot::fillTree(Bot::Node &node, int depth, std::vector<std::vector<int>> board, int turn) {
    if (depth == this->depth) return;

    //Odbudowywanie drzewa
    if (!node.children.empty() && depth != this->depth - 1) {
        for (auto &child : node.children) {
            fillTree(child, depth + 1, board, turn+1);
        }
        return;
    }

    //Sprawdź czy jest bicie
    std::vector<position> moves = BoardActions::available_jumps(board, turn);
    // Ruch
    if (moves.empty()) {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (board[row][col] == 0) continue;
                if (turn % 2 == board[row][col] > 0) continue;
                moves = BoardActions::available_moves_from({row, col}, board, turn);
                for (auto &move: moves) {
                    Node child;
                    std::vector<std::vector<int>> temp_board = board;
                    std::vector<position> temp_move = {{row, col}, move};
                    child.move = BoardActions::position_to_notation(temp_move);
                    child.turn = turn+1;
                    node.children.push_back(child);
                    BoardActions::no_legal_check_move({row, col}, move, temp_board, turn);
                    fillTree(node.children.back(), depth + 1, temp_board, turn+1);
                }
            }
        }
        //Bicie
    } else {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (board[row][col] == 0) continue;
                if (turn % 2 == board[row][col] > 0) continue;
                std::vector<std::vector<position>> sequences;
                BoardActions::available_jump_sequences({row, col},
                                                       {},
                                                       board,
                                                       turn,
                                                       sequences);

                for (auto &seq: sequences) {
                    if(seq.empty()) continue;
                    Node child;
                    std::vector<std::vector<int>> temp_board = board;
                    std::vector<position> temp_move = {{row, col}};
                    for (auto &move: seq) {
                        temp_move.push_back(move);
                    }
                    child.move = BoardActions::position_to_notation(temp_move);
                    child.turn = turn+1;
                    node.children.push_back(child);
                    for (int i = 0; i < temp_move.size() - 1; i++) {
                        BoardActions::no_legal_check_jump(temp_move[i],
                                                          temp_move[i + 1],
                                                          temp_board,
                                                          turn);
                    }
                    //Promocja
                    if ((temp_move.back().row==0 || temp_move.back().row==7) && !BoardActions::is_king(temp_move.back(), board)) {
                        board[temp_move.back().row][temp_move.back().col] *= 2;
                    }
                    fillTree(node.children.back(), depth + 1, temp_board, turn+1);
                }
            }
        }
    }
}

void Bot::removeChildren(Bot::Node &node) {
    for (auto &child: node.children) {
        removeChildren(child);
    }
    node.children.clear();
}

void Bot::collapseTree(std::string &notation) {
    for (auto it = root.children.begin(); it != root.children.end();) {
        if (it->move == notation) {
            ++it;
        } else {
            removeChildren(*it);
            it = root.children.erase(it);
        }
    }
}

void Bot::minmax(Bot::Node &node, int depth, bool is_max, double alpha, double beta, std::vector<std::vector<int>> board) {
    if (depth == this->depth) {
        node.eval = evaluate(node, board);
        return;
    }
    if (is_max) {
        double max_eval = std::numeric_limits<double>::min();
        for (auto &child: node.children) {
            auto temp_board = board;
            BoardActions::no_legal_check_action(child.move, temp_board, child.turn);
            minmax(child, depth + 1, false, alpha, beta, temp_board);
            max_eval = std::max(max_eval, child.eval);
            alpha = std::max(alpha, max_eval);
            node.eval = max_eval;
            if (beta < alpha) break;
        }
        node.eval = max_eval;
    } else {
        double min_eval = std::numeric_limits<double>::max();
        for (auto &child: node.children) {
            auto temp_board = board;
            BoardActions::no_legal_check_action(child.move, temp_board, child.turn);
            minmax(child, depth + 1, true, alpha, beta, temp_board);
            min_eval = std::min(min_eval, child.eval);
            beta = std::min(beta, min_eval);
            node.eval = min_eval;
            if (beta < alpha) break;
        }
        node.eval = min_eval;
    }
}

double Bot::evaluate(Bot::Node &node, const std::vector<std::vector<int>> &board) {
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

std::string Bot::getBestMove() {
    std::vector<std::string> best_moves;
    for(int i=0; i<root.children.size(); i++) {
        if(root.children[i].eval == root.eval) {
            best_moves.emplace_back(root.children[i].move);
        }
    }
    return best_moves[gen() % best_moves.size()];
}


