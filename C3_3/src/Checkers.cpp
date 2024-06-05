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

void Checkers::play() {
    std::string notation;
    while (true) {
        draw_board();
        std::cin >> notation;
        int temp = turn;
        if (!BoardActions::action(notation, board, turn)) {
            result = (temp%2==0) ? WHITE_WIN : BLACK_WIN;
            break;
        }
        if(check_win_condition()) break;
        turn++;
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

void Checkers::play_with_bot(bool is_human_black, int depth, int64_t seed) {
    std::string notation;
    Bot bot(!is_human_black, depth, seed);
    while (true) {
        if(turn%2==!is_human_black) {
            draw_board();
            std::cin >> notation;
            int temp = turn;
            if (!BoardActions::action(notation, board, turn)) {
                result = (temp % 2 == 0) ? WHITE_WIN : BLACK_WIN;
                break;
            }
            if (check_win_condition()) break;
            turn++;
        }
        draw_board();
        bot.collapseTree(notation);
        bot.fillTree(bot.root, 0, board, turn);
        bot.minmax(bot.root,
                   0, true,
                   std::numeric_limits<double>::min(),
                   std::numeric_limits<double>::max(),
                   board);
        notation = bot.getBestMove();
        int temp = turn;
        bot.collapseTree(notation);
        if (!BoardActions::action(notation, board, turn)) {
            result = (temp%2==0) ? WHITE_WIN : BLACK_WIN;
            break;
        }
        if(check_win_condition()) break;
        turn++;
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

/*void Checkers::bot_vs_bot() {
    using namespace std::chrono;
    std::string notation;
    std::random_device rand;
    Bot bot_black(true, 8, 232, TWENTY_FIVE_PARAMS);
    Bot bot_white(false, 8, 577, TWENTY_FIVE_PARAMS);
    long max_time=-1;
    while (true) {
        draw_board();
        auto start_black = high_resolution_clock::now();
        notation = bot_black.move(board, turn, notation);
        std::cout << notation << std::endl;
        std::cout << "Eval: " << bot_black.root.eval << std::endl;
        int temp = turn;
        auto stop_black = high_resolution_clock::now();
        auto duration_black = duration_cast<milliseconds>(stop_black - start_black);
        std::cout << "Black move took " << duration_black.count() << "ms" << std::endl;

        if (!BoardActions::action(notation, board, turn)) {
            result = (temp%2==0) ? WHITE_WIN : BLACK_WIN;
            break;
        }
        if(check_win_condition()) break;
        turn++;
        draw_board();
        auto start_white = high_resolution_clock::now();
        notation = bot_white.move(board, turn, notation);
        temp = turn;
        auto stop_white = high_resolution_clock::now();
        auto duration_white = duration_cast<milliseconds>(stop_white - start_white);
        std::cout << "White move took " << duration_white.count() << "ms" << std::endl;
        if (!BoardActions::action(notation, board, turn)) {
            result = (temp%2==0) ? WHITE_WIN : BLACK_WIN;
            break;
        }
        if(check_win_condition()) break;
        turn++;
        max_time = std::max(max_time, std::max(duration_black.count(), duration_white.count()));
    }
    std::cout << "Max time: " << max_time << "ms" << std::endl;
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
}*/


bool Checkers::check_win_condition() {
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

void Checkers::train() {
    std::random_device rand;
    //starting parameters
    std::vector<double> parameters {
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
    };
    std::vector<double> prev_parameters;
    board = { { 0, 1,  0, 1,  0, 1,  0, 1},
              { 1, 0,  1, 0,  1, 0,  1, 0},
              { 0, 1,  0, 1,  0, 1,  0, 1},
              { 0, 0,  0, 0,  0, 0,  0, 0},
              { 0, 0,  0, 0,  0, 0,  0, 0},
              {-1, 0, -1, 0, -1, 0, -1, 0},
              { 0,-1,  0,-1,  0,-1,  0,-1},
              {-1, 0, -1, 0, -1, 0, -1, 0} };
    Bot bot1(true, 8, 0, TWENTY_FIVE_PARAMS);
    Bot bot2(false, 8, rand(), SIMPLE);


}

// Getters
int Checkers::getTurn() const {
    return turn;
}




