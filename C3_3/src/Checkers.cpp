#include <fstream>
#include "../inc/Checkers.h"


Checkers::Checkers(std::vector<std::vector<int>> &board) : board(board){
    turn = 0;
    no_jumps_count = 0;
    result =  NONE;
}

void Checkers::draw_board() {
    int count = 0;
    std::cout << "Turn " << turn << std::endl;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (board[row][col] == 0) {
                if((row+col)%2 == 0) std::cout << "     ";
                else {
                    count++;
                    std::cout << "(" << count << (count < 10 ? " " : "") << " )";

                }
            } else if (std::abs(board[row][col]) == 1) {
                count++;
                std::cout << "(" << count << (count < 10 ? " " : "")
                          << (board[row][col] > 0 ? "\u26C0" : "\u26C2") << ")";
            } else {
                count++;
                std::cout << "(" << count << (count < 10 ? " " : "")
                          << (board[row][col] > 0 ? "\u26C1" : "\u26C3") << ")";;
            }
        }
        std::cout << '\n' << std::endl;
    }
    std::cout << "Enter move: ";
}

void Checkers::play_with_bot(bool is_bot_black, Bot &bot) {
    std::string notation;
    while (true) {
        draw_board();
        if(is_bot_black) {
            notation = bot.move(board, turn, notation);
            std::cout << "Ruch bota: " << notation << std::endl;
            std::cout << notation << std::endl;
            if (!make_move(notation)) break;
        } else {
            std::cout << "Ruch gracza: ";
            std::cin >> notation;
            if (!make_move(notation)) break;
        }

        draw_board();
        if(!is_bot_black) {
            notation = bot.move(board, turn, notation);
            std::cout << "Ruch bota: " << notation << std::endl;
            if (!make_move(notation)) break;
        } else {
            std::cout << "Ruch gracza: ";
            std::cin >> notation;
            if (!make_move(notation)) break;
        }
    }

}

GameResult Checkers::bot_vs_bot(Bot &bot_black, Bot &bot_white) {
    std::string notation;
    std::random_device rand;
    while (true) {

//        draw_board();
        notation = bot_black.move(board, turn, notation);
//        std::cout << notation << std::endl;
//        std::cout << "Black eval: " << bot_black.getEval() << std::endl;
        if (!make_move(notation)) break;

//        draw_board();
        notation = bot_white.move(board, turn, notation);
//        std::cout << notation << std::endl;
//        std::cout << "White eval: " << bot_white.getEval() << std::endl;
        if (!make_move(notation)) break;
    }
    return result;
}


bool Checkers::check_win_condition() {

    if(no_jumps_count==20) {
        result = DRAW;
        return true;
    }
    //Jeśli nie ma już pionków jednego z graczy
    int black_count = 0;
    int white_count = 0;
    for(auto &row : board) {
        for(auto &field : row) {
            if(field>0) black_count++;
            if(field<0) white_count++;
        }
    }
    if(black_count==0) result = WHITE_WIN;
    else if(white_count==0) result = BLACK_WIN;

    //Jeśli po wykonaniu ruchu przeciwnik nie ma możliwości ruchu
    if(BoardActions::available_jumps(board, turn+1).empty() and BoardActions::available_moves(board, turn+1).empty()) {
        result = (turn%2==0) ? BLACK_WIN : WHITE_WIN;
    }
    //Jeśli wszystko jest ok, gra dalej trwa
    if(result!=NONE) return true;

    return false;
}

void Checkers::train() {
    std::ofstream file;
    std::random_device rand;

    const size_t max_ladder_size = 32;
    const size_t min_ladder_size = 4;
    //starting parameters
    size_t ladder_size = max_ladder_size;
    std::vector<std::vector<double>> parameters {ladder_size, {
            1.0, //number_of_pawns
            1.8, //number_of_kings
            -0.05, //total_distance_to_promotion
            0.3, //save_pawn
            0.6, //save_king
            0.4, //defending_piece
            0.6, //attacking_pawn
            0.5, //central_pawn
            1.1, //central_king
            0.1, //movable_pawn
            0.2, //movable_king
            0.1, //empty_promotion_fields
            0.2, //main_diagonal_pawn
            0.5, //main_diagonal_king
            0.15, //double_diagonal_pawn
            0.30, //double_diagonal_king
            -0.2, //loner_pawn
            -0.1, //loner_king
            0.2, //holes
            0.3, //is_triangle_pattern
            0.3, //is_oreo_pattern
            0.4, //is_bridge_pattern
            -0.1, //is_dog_pattern
            -0.1, //is_corner_pawn
            -0.1, //is_corner_king
    }};
    for(auto &param : parameters) {
        for (double & p : param) {
            p *= 0.5 + ((rand() % 1000) / 1000.0);
        }
    }
    std::set <size_t> losers;
    int win_count = 0;
    resetBoard();
    int generation = 0;
    while(true) {
        std::string name = "../parameters/" + std::to_string(generation) + ".txt";
        file.open(name, std::ios::app);

        while (ladder_size != min_ladder_size) {
            for (int param = 0; param < ladder_size - 1; param += 2) {

                Bot bot1(true, 8, rand(), TWENTY_FIVE_PARAMS);
                Bot bot2(false, 8, rand(), TWENTY_FIVE_PARAMS);
                bot1.setParameters(parameters[param]);
                bot2.setParameters(parameters[param + 1]);
                bot_vs_bot(bot1, bot2);
                if (result == BLACK_WIN) {
                    win_count++;
                } else if (result == WHITE_WIN) {
                    win_count--;
                }
                bot1.switchSide();
                bot2.switchSide();
                bot1.clearRoot();
                bot2.clearRoot();
                resetBoard();
                bot_vs_bot(bot2, bot1);
                if (result == BLACK_WIN) {
                    win_count--;
                } else if (result == WHITE_WIN) {
                    win_count++;
                }
                if (win_count > 0) {
                    losers.insert(param + 1);
                } else if (win_count < 0) {
                    losers.insert(param);
                } else {
                    losers.insert(rand() % 2 ? param + 1 : param);
                }
                bot1.switchSide();
                bot2.switchSide();
                bot1.clearRoot();
                bot2.clearRoot();
                resetBoard();
            }
            for (auto it = losers.rbegin(); it != losers.rend(); it++) {
                parameters.erase(parameters.begin() + *it);
            }
            losers.clear();
            ladder_size /= 2;
        }
        std::cout << "Generation " << generation << std::endl;
        for(auto &param : parameters) {
            for(auto &p : param) {
                file << std::to_string(p) << " ";
            }
            std::cout << std::endl;
        }
        generation++;
        //Stworzenie wariacji zwycięzców i uzupełnienie spowrotem do 64
        for(size_t i = 0; i<max_ladder_size-min_ladder_size; i++) {
            parameters.emplace_back(parameters[i%min_ladder_size]);
            for (double & p : parameters.back()) {
                p *= 0.5 + ((rand() % 1000) / 1000.0);
            }
        }
        ladder_size = max_ladder_size;
        file.close();
    }

}




void Checkers::train_2() {
    std::vector<double> params = {3.83707, 8.38775, -0.0578258, 0.14021, 0.301927, 0.67968, 0.300158, 0.426312, 0.571157, 0.0678951, 0.134511, 0.223155, 0.11514, 0.361015, 0.0271743, 0.100025, -0.159225, -0.0498955, 0.0945398, 0.480581, 0.313735, 0.142473, -0.0664429, -0.0624859, -0.0971413};

    Bot bot1(true, 8, 5, TWENTY_FIVE_PARAMS);
    Bot bot2(false, 8, 6, SIMPLE);
    int prev_performance = 0;
    for(int iteration=0; iteration<10; iteration++) { //iteracja
        for(int i=params.size()-1; i>=0; i--) { //petla po kazdym parametrze
            bool didIncrease = true;
            bool didPrevIncrease = true;
            double learning_rate = 0.1*params[i];
            for(int feedback=0; feedback<6; feedback++) { //pętla sprzężenia zwrotnego
                bot1.setParameters(params);
                bot_vs_bot(bot1, bot2);
                int performance = Evaluation::performance(true, turn, result, board);
                //Sprzezenie zwrotne
                if(performance>prev_performance) {
                    didIncrease = true;
                } else {
                    didIncrease = false;
                }
                if(didPrevIncrease==didIncrease) {
                    learning_rate *= 1.2;
                } else {
                    learning_rate *= 0.9;
                }
                params[i] += (didIncrease ? 1 : -1) * learning_rate;

                std::cout << "Performance: " << performance << std::endl;
                prev_performance = performance;
                didPrevIncrease = didIncrease;
                draw_board();
                resetBoard();

            }
            std::cout << params[i] << std::endl;
        }
        for(auto param : params) {
            std::cout << param << ", ";
        }
        std::cout << std::endl;
    }

}

bool Checkers::make_move(const std::string &notation) {
    if (!BoardActions::action(notation, board, turn, no_jumps_count)) {
        result = (turn%2==0) ? WHITE_WIN : BLACK_WIN;
        return false;
    }
    if(!check_win_condition()) {
        this->turn++;
        return true;
    }
    switch (result) {
        case BLACK_WIN:
            std::cout << "Black wins" << std::endl;
            return false;
        case WHITE_WIN:
            std::cout << "White wins" << std::endl;
            return false;
        case DRAW:
            std::cout << "Draw" << std::endl;
            return false;
        case NONE:
            //Nie powinno się zdarzyć
            std::cout << "???" << std::endl;
            break;
    }
    return true;
}

void Checkers::resetBoard() {
    board = { { 0, 1,  0, 1,  0, 1,  0, 1},
              { 1, 0,  1, 0,  1, 0,  1, 0},
              { 0, 1,  0, 1,  0, 1,  0, 1},
              { 0, 0,  0, 0,  0, 0,  0, 0},
              { 0, 0,  0, 0,  0, 0,  0, 0},
              {-1, 0, -1, 0, -1, 0, -1, 0},
              { 0,-1,  0,-1,  0,-1,  0,-1},
              {-1, 0, -1, 0, -1, 0, -1, 0} };
    turn = 0;
    no_jumps_count = 0;
    result = NONE;
}

std::string Checkers::give_move(Bot &bot, const std::string& notation) {
    return bot.move(board, turn, notation);
}






