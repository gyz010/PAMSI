#include <iostream>
#include "../inc/Checkers.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <strings.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSPACE 1024
#define BLACK 1
#define WHITE 0

int errno;

void play_net(int argc, char* argv[]) {
    std::vector<std::vector<int>> board = { { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 1, 0,  1, 0,  1, 0,  1, 0},
                                            { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            {-1, 0, -1, 0, -1, 0, -1, 0},
                                            { 0,-1,  0,-1,  0,-1,  0,-1},
                                            {-1, 0, -1, 0, -1, 0, -1, 0} };
    Checkers checkers(board);
    char buf[BUFSPACE];
    int player, serv_sock, n, nr_ruchu;
    int depth;
    int64_t seed;
    struct sockaddr_in serv_addr;
    struct hostent *serv_hostent;
    /* taka sobie przykladowa gra */
    std::vector<std::string> ruchy;

    /* argv[2] okresla ktora strona ma grac program */
    player = -1;
    if (strcasecmp(argv[2], "BLACK") == 0) player = BLACK;
    if (strcasecmp(argv[2], "WHITE") == 0) player = WHITE;
    if (player < 0) {
        fprintf(stderr, "%s: niepoprawne okreslenie gracza w argv[2]: %s\n",
                argv[0], argv[2]);
        exit(-1);
    }

    /* argv[3] zadaje glebokosc przeszukiwania minimaksu */
    depth = std::stoi(argv[3]);

    /* argv[4] zadaje ziarno generatora liczb losowych */
    if(argc>4)
        seed = std::stoll(argv[4]);
    else {
        std::random_device rand;
        seed = rand();
    }
    /* argv[5] zadaje nazwe serwera gry */
    /* argv[6] zadaje numer portu serwera gry */
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0) {
        perror("socket");
        exit(errno);
    }
    serv_hostent = gethostbyname(argv[5]);
    if (serv_hostent == 0) {
        fprintf(stderr, "%s: nieznany adres IP %s\n", argv[0], argv[5]);
        exit(-1);
    }
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr, serv_hostent->h_addr, serv_hostent->h_length);
    serv_addr.sin_port = htons(atoi(argv[6]));

    printf("Laczymy sie z serwerem ...\n");
    if (connect(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(-1);
    }
    std::vector<double> params  = {5.145784,
                                   8.921238,
                                   -0.054288,
                                   0.205565,
                                   0.308606,
                                   0.305944,
                                   0.461340,
                                   0.533956,
                                   0.575005,
                                   0.111109,
                                   0.215599,
                                   0.175594,
                                   0.208280,
                                   0.542531,
                                   0.047016,
                                   0.201801,
                                   -0.129941,
                                   -0.065280,
                                   0.092407,
                                   0.514447,
                                   0.352783,
                                   0.357689,
                                   -0.076711,
                                   -0.109690,
                                   -0.092557
    };
    Bot bot(player == BLACK, depth, seed, TWENTY_FIVE_PARAMS);
    bot.setParameters(params);


    printf("Polaczenie nawiazane, zaczynamy gre ...\n");

    nr_ruchu = 0;				/* czarny robi ruchy parzyste */
    if (player==WHITE) nr_ruchu = 0;	/* bialy nieparzyste */
    std::string notation;
    while(1) {
        if ((nr_ruchu>0) || (player==BLACK)) {
            //Tutaj wykonuje wszystko zwiazane z ruchem
            notation = checkers.give_move(bot, notation);
            checkers.make_move(notation);
            printf("Wysylam do serwera moj ruch: %s\n", notation.c_str());
            //Wysylam ruch
            if (write(serv_sock, notation.c_str(), notation.size()) < 0){
                perror("write");
                exit(errno);
            }
        }
        printf("Czekam na ruch przeciwnika ...\n");
        n=read(serv_sock, buf, sizeof buf);
        if (n<0) {
            perror("read");
            exit(errno);
        }
        if (n==0) { /* pusty komunikat = zamkniete polaczenie */
            printf("Broker zamknal polaczenie, hmmmm...\n");
            exit(0);
        }
        buf[n] = 0;
        printf("Otrzymalem ruch przeciwnika: %s", buf);
        if (buf[n-1]!='\n') printf("\n");
        //ustawiam go na planszy
        notation = buf;
        checkers.make_move(notation);
        ++nr_ruchu;
    }
}

void play_gui(int argc, char *argv[]) {
    std::vector<std::vector<int>> board = { { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 1, 0,  1, 0,  1, 0,  1, 0},
                                            { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            {-1, 0, -1, 0, -1, 0, -1, 0},
                                            { 0,-1,  0,-1,  0,-1,  0,-1},
                                            {-1, 0, -1, 0, -1, 0, -1, 0} };
    Checkers checkers(board);
    bool is_bot_black;
    //Arg2 Program Black or White
    if(argv[2] == std::string("BLACK")) is_bot_black = true;
    else if(argv[2] == std::string("WHITE")) is_bot_black = false;
    else throw std::invalid_argument("arg2: Możliwe opcje to BLACK lub WHITE");

    //Arg3 Depth
    int depth = std::stoi(argv[3]);
    //Arg4 Seed
    int64_t seed;
    if(argc>4) {
        seed = std::stoll(argv[4]);
    }
    else {
        std::random_device rand;
        seed = rand();
    }

    Bot bot(is_bot_black, depth, seed, SIMPLE);
    checkers.play_with_bot(is_bot_black, bot);
}


int main(int argc, char* argv[]) {

    /* argv[1] okresla czy gramy lokalnie GUI czy zdalnie NET */
//    if(argv[1] == std::string("NET")) {
//        play_net(argc, argv);
//        return 0;
//    }
//    else if(argv[1] == std::string("GUI")) {
//        play_gui(argc, argv);
//    }



    std::vector<std::vector<int>> board = { { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 1, 0,  1, 0,  1, 0,  1, 0},
                                            { 0, 1,  0, 1,  0, 1,  0, 1},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            { 0, 0,  0, 0,  0, 0,  0, 0},
                                            {-1, 0, -1, 0, -1, 0, -1, 0},
                                            { 0,-1,  0,-1,  0,-1,  0,-1},
                                            {-1, 0, -1, 0, -1, 0, -1, 0} };

    std::random_device rand;
    Checkers checkers(board);
    std::vector<double> params = {1.0, //number_of_pawns
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
    /*Bot bot(true, 8, rand(), TWENTY_FIVE_PARAMS);
    bot.setParameters(params);
    Bot bot2(false, 8, rand(), SIMPLE);
    int draws=0;
    int black_wins=0;
    int white_wins=0;
    for(int i = 0; i < 100; i++) {
        GameResult result = checkers.bot_vs_bot(bot, bot2);
        checkers.resetBoard();
        if(result == BLACK_WIN) black_wins++;
        else if(result == WHITE_WIN) white_wins++;
        else if(result == DRAW) draws++;
        std::cout << "Black winrate: " << (double)black_wins/(i+1) << std::endl;
        std::cout << "Black draw/winrate: " << (double)(draws+black_wins)/(i+1) << std::endl;
    }*/
    checkers.train_2();


}
