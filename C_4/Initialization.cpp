//
// Created by gyz on 23.06.24.
//

#include "Initialization.h"

Args Initialization::args;

Algorithm Initialization::choose_algorithm(const std::string &s) {
    if (s == std::string("NAIVE")) {
        return NAIVE;
    } else if (s == std::string("RK")) {
        return RABIN_KARP;
    } else if (s == std::string("KMP")) {
        return KMP;
    } else if (s == std::string("BM")) {
        return BOYER_MOORE;
    } else if (s == std::string("SA"))  {
        return SUFFIX_ARRAY;
    } else {
        throw std::invalid_argument("Invalid algorithm");
    }
}

/// Arguments: ./main [main_string_file] [sub_string] [algorithm] [bool:case_sensitivity] [bool:ignore_whitespace]
/// Arguments: ./main [test] [algorithm] [bool:case_sensitivity] [bool:ignore_whitespace]
void Initialization::handle_args(int argc, char **argv) {
    if(argc != 6 && argc!=5) {
        throw std::invalid_argument("Invalid number of arguments");
    }
    if(argc == 6) {
        args.main_string_file = argv[1];
        args.sub_string = argv[2];

        std::string s = argv[3];
        args.algorithm = choose_algorithm(s);

        args.case_sensitivity = std::stoi(argv[4]) == 1;
        args.ignore_whitespace = std::stoi(argv[5]) == 1;
    }
    else {
        args.test = argv[1];
        std::string s = argv[2];
        args.algorithm = choose_algorithm(s);

        args.case_sensitivity = std::stoi(argv[3]) == 1;
        args.ignore_whitespace = std::stoi(argv[4]) == 1;
    }
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

void Initialization::remove_whitespace(std::string &str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

void Initialization::to_lower(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}
