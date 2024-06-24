//
// Created by gyz on 23.06.24.
//

#include "Initialization.h"

Args Initialization::handle_args(int argc, char **argv) {
    Args args;
    if(argc != 6) {
        throw std::invalid_argument("Invalid number of arguments");
    }
    args.main_string_file = argv[1];
    args.sub_string = argv[2];

    std::string s = argv[3];

    if(argv[3] == std::string("NAIVE")) {
        args.algorithm = NAIVE;
    } else if(argv[3] == std::string("RK")) {
        args.algorithm = RABIN_KARP;
    } else if(argv[3] == std::string("KMP")) {
        args.algorithm = KMP;
    } else if(argv[3] == std::string("BM")) {
        args.algorithm = BOYER_MOORE;
    } else {
        throw std::invalid_argument("Invalid algorithm");
    }

    args.case_sensitivity = std::stoi(argv[4]) == 1;
    args.ignore_whitespace = std::stoi(argv[5]) == 1;

    return args;
}

std::string Initialization::read_to_main_string(const std::string &main_string_file) {
    std::ifstream main_file(main_string_file);

    if(!main_file.is_open()) {
        throw std::invalid_argument("Invalid file path");
    }
    //https://stackoverflow.com/a/2602060 - odczyt całego pliku do stringa
    std::ifstream file(main_string_file);
    std::string main((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    main_file.close();
    return main;

}

void Initialization::remove_whitespace(std::string &main, std::string &sub) {
    main.erase(std::remove_if(main.begin(), main.end(), ::isspace), main.end());
    sub.erase(std::remove_if(sub.begin(), sub.end(), ::isspace), sub.end());
}

void Initialization::to_lower(std::string &main, std::string &sub) {
    std::transform(main.begin(), main.end(), main.begin(), ::tolower);
    std::transform(sub.begin(), sub.end(), sub.begin(), ::tolower);
}
