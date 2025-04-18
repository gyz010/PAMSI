//
// Created by gyz on 20.05.24.
//

#include "../inc/Bot.h"


void Bot::fillTree(Bot::Node &node, int depth, std::vector<std::vector<int>> &board, int turn) {
    if (depth == this->depth) return;

//    Odbudowywanie drzewa
    if (!node.children.empty() && depth+1 < this->depth) {
        for (auto &child : node.children) {
            bool promoted = false;
            std::vector<int> taken_pieces;
            BoardActions::no_legal_check_action(child.move, board, taken_pieces, promoted);
            fillTree(child, depth + 1, board, turn + 1);
            BoardActions::undo_action(child.move, board, taken_pieces, promoted);
        }
        return;
    }

    //Sprawdź czy jest bicie
    std::vector<position> jumps_from;
    std::vector<position> moves = BoardActions::available_jumps(board, turn, jumps_from);
    // Ruch
    if (moves.empty()) {
        for (int row = 0; row < 8; ++row) {
            for (int col = (row + 1) % 2; col < 8; col += 2) {
                if (board[row][col] == 0) continue;
                if (turn % 2 == board[row][col] > 0) continue;
                moves = BoardActions::available_moves_from({row, col}, board, turn);
                node.children.reserve(moves.size());
                for (auto &move: moves) {
                    //Tworzenie dzieci, nadawanie im ich ruchów.
                    std::vector<position> temp_move = {{row, col}, move};
                    Node child(BoardActions::position_to_notation(temp_move), turn+1);
                    node.children.emplace_back(child);

                    //Wykonanie ruchu i przejscie do kolejnego poziomu drzewa
                    bool promoted = false;
                    BoardActions::no_legal_check_move({row, col}, move, board, promoted);
                    fillTree(node.children.back(), depth + 1, board, turn + 1);
                    //Cofanie ruchu po wyjściu z rekurencji
                    BoardActions::undo_move({row, col}, move, board, promoted);
                }
            }
        }
    }
        //Bicie
    else {
        for(auto &from : jumps_from) {
            std::vector<std::vector<position>> sequences;
            BoardActions::available_jump_sequences(from,
                                                   {},
                                                   board,
                                                   turn,
                                                   sequences);
            for (auto &seq: sequences) {
                if(seq.empty()) continue;
                //Tworzenie dzieci, nadawanie im ich ruchów.
                std::vector<position> temp_move = {from};
                for (auto &jump: seq) {
                    temp_move.push_back(jump);
                }
                Node child(BoardActions::position_to_notation(temp_move), turn+1);
                node.children.emplace_back(child);
                //Wykonanie bić i przejscie do kolejnego poziomu drzewa
                bool promoted = false;
                std::vector<int> taken_pieces;
                for (int i = 0; i < temp_move.size() - 1; i++) {
                    BoardActions::no_legal_check_jump(temp_move[i],
                                                      temp_move[i + 1],
                                                      board,
                                                      taken_pieces,
                                                      promoted);
                }
                fillTree(node.children.back(), depth + 1, board, turn + 1);
                //Cofanie bić po wyjściu z rekurencji
                BoardActions::undo_jump(temp_move, board, taken_pieces, promoted);
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

void Bot::collapseTree(const std::string &notation) {
    removeChildren(root);
    root.move=notation;
}


void Bot::minmax(Bot::Node &node, int depth, bool is_max, double alpha, double beta, std::vector<std::vector<int>> &board) {

    if (depth == this->depth or node.children.empty()) {
        switch (eval_type) {
            case RANDOM:
                node.eval = Evaluation::evaluate_random();
                break;
            case SIMPLE:
                node.eval = Evaluation::evaluate_simple(board, is_black);
                break;
            case TWENTY_FIVE_PARAMS:
                node.eval = Evaluation::evaluate_25(board, is_black, parameters);
                break;
        }
        return;
    }
    if (is_max) {
        double max_eval = -100000;
        for (auto &child: node.children) {
            bool promoted = false;
            std::vector<int> taken_pieces;
            BoardActions::no_legal_check_action(child.move, board, taken_pieces, promoted);
            minmax(child, depth + 1, false, alpha, beta, board);
            BoardActions::undo_action(child.move, board, taken_pieces, promoted);

            max_eval = std::max(max_eval, child.eval);
            alpha = std::max(alpha, max_eval);
            node.eval = max_eval;
            if (beta < alpha) break;
        }
    } else {
        double min_eval = 100000;
        for (auto &child: node.children) {
            bool promoted = false;
            std::vector<int> taken_pieces;
            BoardActions::no_legal_check_action(child.move, board, taken_pieces, promoted);
            minmax(child, depth + 1, true, alpha, beta, board);
            BoardActions::undo_action(child.move, board, taken_pieces, promoted);
            min_eval = std::min(min_eval, child.eval);
            beta = std::min(beta, min_eval);
            node.eval = min_eval;
            if (beta < alpha) break;
        }
    }

}


/*void Bot::fill_tree_with_minmax(Bot::Node &node,
                           std::vector<std::vector<int>> &board,
                           int depth,
                           int turn,
                           bool is_max,
                           double alpha,
                           double beta) {

    if (depth == this->depth) {
        switch (eval_type) {
            case RANDOM:
                node.eval = Evaluation::evaluate_random();
                break;
            case SIMPLE:
                node.eval = Evaluation::evaluate_simple(board, is_black);
                break;
            case ELEVEN_PARAMS:
                node.eval = Evaluation::evaluate_11(board, is_black);
                break;
            case TWENTY_FIVE_PARAMS:
                node.eval = Evaluation::evaluate_25(board, is_black, parameters);
                break;
        }
        return;
    }

    //Sprawdź czy jest bicie
    std::vector<position> jumps_from;
    std::vector<position> moves = BoardActions::available_jumps(board, turn, jumps_from);
    // Ruch
    if (moves.empty()) {
        for (int row = 0; row < 8; ++row) {
            for (int col = (row + 1) % 2; col < 8; col += 2) {
                if (board[row][col] == 0) continue;
                if (turn % 2 == board[row][col] > 0) continue;
                moves = BoardActions::available_moves_from({row, col}, board, turn);
                node.children.reserve(moves.size());
                for (auto &move: moves) {
                    //Tworzenie dzieci, nadawanie im ich ruchów.
                    std::vector<position> temp_move = {{row, col}, move};
                    Node child(BoardActions::position_to_notation(temp_move), turn+1);
                    node.children.emplace_back(child);

                    //Wykonanie ruchu i przejscie do kolejnego poziomu drzewa
                    bool promoted = false;
                    BoardActions::no_legal_check_move({row, col}, move, board, promoted);
                    double max_eval = -100000;
                    double min_eval = 100000;
                    if(is_max) {
                        fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, false, alpha, beta);
                        BoardActions::undo_move({row, col}, move, board, promoted);
                        max_eval = std::max(max_eval, node.children.back().eval);
                        alpha = std::max(alpha, max_eval);
                        node.eval = max_eval;
                        if (beta < alpha) break;
                    } else {
                        fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, true, alpha, beta);
                        BoardActions::undo_move({row, col}, move, board, promoted);
                        min_eval = std::min(min_eval, node.children.back().eval);
                        beta = std::min(beta, min_eval);
                        node.eval = min_eval;
                        if (beta < alpha) break;
                    }
                }
            }
        }
    }
        //Bicie
    else {
        for(auto &from : jumps_from) {
            std::vector<std::vector<position>> sequences;
            BoardActions::available_jump_sequences(from,
                                                   {},
                                                   board,
                                                   turn,
                                                   sequences);
            for (auto &seq: sequences) {
                if(seq.empty()) continue;
                //Tworzenie dzieci, nadawanie im ich ruchów.
                std::vector<position> temp_move = {from};
                for (auto &jump: seq) {
                    temp_move.push_back(jump);
                }
                Node child(BoardActions::position_to_notation(temp_move), turn+1);
                node.children.emplace_back(child);
                //Wykonanie bić i przejscie do kolejnego poziomu drzewa
                bool promoted = false;
                std::vector<int> taken_pieces;
                for (int i = 0; i < temp_move.size() - 1; i++) {
                    BoardActions::no_legal_check_jump(temp_move[i],
                                                      temp_move[i + 1],
                                                      board,
                                                      taken_pieces,
                                                      promoted);
                }
                double max_eval = -100000;
                double min_eval = 100000;
                if(is_max) {
                    fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, false, alpha, beta);
                    BoardActions::undo_jump(temp_move, board, taken_pieces, promoted);
                    max_eval = std::max(max_eval, node.children.back().eval);
                    alpha = std::max(alpha, max_eval);
                    node.eval = max_eval;
                    if (beta < alpha) break;
                } else {
                    fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, true, alpha, beta);
                    BoardActions::undo_jump(temp_move, board, taken_pieces, promoted);
                    min_eval = std::min(min_eval, node.children.back().eval);
                    beta = std::min(beta, min_eval);
                    node.eval = min_eval;
                    if (beta < alpha) break;
                }
            }
        }
    }
    if (node.children.empty()) {
        switch (eval_type) {
            case RANDOM:
                node.eval = Evaluation::evaluate_random();
                break;
            case SIMPLE:
                node.eval = Evaluation::evaluate_simple(board, is_black);
                break;
            case ELEVEN_PARAMS:
                node.eval = Evaluation::evaluate_11(board, is_black);
                break;
            case TWENTY_FIVE_PARAMS:
                node.eval = Evaluation::evaluate_25(board, is_black, parameters);
                break;
        }
        return;
    }
}*/

void Bot::fill_tree_with_minmax(Bot::Node &node,
                                std::vector<std::vector<int>> &board,
                                int depth,
                                int turn,
                                bool is_max,
                                double alpha,
                                double beta) {

    if (depth == this->depth) {
        switch (eval_type) {
            case RANDOM:
                node.eval = Evaluation::evaluate_random();
                break;
            case SIMPLE:
                node.eval = Evaluation::evaluate_simple(board, is_black);
                break;
            case TWENTY_FIVE_PARAMS:
                node.eval = Evaluation::evaluate_25(board, is_black, parameters);
                break;
        }
        return;
    }

    //Sprawdź czy jest bicie
    std::vector<position> jumps_from;
    std::vector<position> moves = BoardActions::available_jumps(board, turn, jumps_from);
    bool no_moves = true;

    if (moves.empty()) {
        for (int row = 0; row < 8; ++row) {
            for (int col = (row + 1) % 2; col < 8; col += 2) {
                if (board[row][col] == 0) continue;
                if (turn % 2 == board[row][col] > 0) continue;
                moves = BoardActions::available_moves_from({row, col}, board, turn);
                if (!moves.empty()) {
                    no_moves = false;
                }
                for (auto &move: moves) {
                    //Tworzenie dzieci, nadawanie im ich ruchów.
                    std::vector<position> temp_move = {{row, col}, move};
                    Node child(BoardActions::position_to_notation(temp_move), turn + 1);
                    node.children.emplace_back(child);

                    //Wykonanie ruchu i przejscie do kolejnego poziomu drzewa
                    bool promoted = false;
                    BoardActions::no_legal_check_move({row, col}, move, board, promoted);
                    if (is_max) {
                        fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, false, alpha, beta);
                        BoardActions::undo_move({row, col}, move, board, promoted);
                        alpha = std::max(alpha, node.children.back().eval);
                        if (alpha >= beta) break; // Corrected pruning condition
                    } else {
                        fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, true, alpha, beta);
                        BoardActions::undo_move({row, col}, move, board, promoted);
                        beta = std::min(beta, node.children.back().eval);
                        if (alpha >= beta) break; // Corrected pruning condition
                    }
                }
            }
        }
    } else {
        no_moves = false;
        for (auto &from : jumps_from) {
            std::vector<std::vector<position>> sequences;
            BoardActions::available_jump_sequences(from, {}, board, turn, sequences);
            for (auto &seq: sequences) {
                if (seq.empty()) continue;
                //Tworzenie dzieci, nadawanie im ich ruchów.
                std::vector<position> temp_move = {from};
                for (auto &jump: seq) {
                    temp_move.push_back(jump);
                }
                Node child(BoardActions::position_to_notation(temp_move), turn + 1);
                node.children.emplace_back(child);
                //Wykonanie bić i przejscie do kolejnego poziomu drzewa
                bool promoted = false;
                std::vector<int> taken_pieces;
                for (int i = 0; i < temp_move.size() - 1; i++) {
                    BoardActions::no_legal_check_jump(temp_move[i], temp_move[i + 1], board, taken_pieces, promoted);
                }
                if (is_max) {
                    fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, false, alpha, beta);
                    BoardActions::undo_jump(temp_move, board, taken_pieces, promoted);
                    alpha = std::max(alpha, node.children.back().eval);
                    if (beta < alpha) break;
                } else {
                    fill_tree_with_minmax(node.children.back(), board, depth + 1, turn + 1, true, alpha, beta);
                    BoardActions::undo_jump(temp_move, board, taken_pieces, promoted);
                    beta = std::min(beta, node.children.back().eval);
                    if (beta < alpha) break;
                }
            }
        }
    }

    if (no_moves) {
        switch (eval_type) {
            case RANDOM:
                node.eval = Evaluation::evaluate_random();
                break;
            case SIMPLE:
                node.eval = Evaluation::evaluate_simple(board, is_black);
                break;
            case TWENTY_FIVE_PARAMS:
                node.eval = Evaluation::evaluate_25(board, is_black, parameters);
                break;
        }
        return;
    }

    if (is_max) {
        node.eval = alpha;
    } else {
        node.eval = beta;
    }
}



std::string Bot::getBestMove() {
    std::vector<std::string> best_moves;
    for(int i=0; i<root.children.size(); i++) {
        if(root.children[i].eval == root.eval) {
            best_moves.emplace_back(root.children[i].move);
        }
    }
    if(best_moves.empty()) {
        return root.children[gen() % root.children.size()].move;
    }
    return best_moves[gen() % best_moves.size()];
}

std::string Bot::move(std::vector<std::vector<int>> &board, int turn, const std::string &notation) {
    collapseTree(notation);
//    fillTree(root, 0, board, turn);
//    minmax(root, 0, true, -100000, 100000, board);
    fill_tree_with_minmax(root, board, 0, turn, true, -100000, 100000);
    std::string move_notation = getBestMove();
    collapseTree(move_notation);
    return move_notation;
}
