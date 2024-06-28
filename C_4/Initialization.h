//
// Created by gyz on 23.06.24.
//

#ifndef C_4_INITIALIZATION_H
#define C_4_INITIALIZATION_H

#include "StringSearch.h"
#include <fstream>
#include <algorithm>

struct Args {
    std::string main_string_file;
    std::string sub_string;
    Algorithm algorithm;
    bool case_sensitivity;
    bool ignore_whitespace;
    std::string test;
    Args() {
        algorithm = NAIVE;
        case_sensitivity = false;
        ignore_whitespace = false;

    }
};

class Initialization {
private:
    static Algorithm choose_algorithm(const std::string &s);
    static Args args;
public:
    static void handle_args(int argc, char **argv);
    static std::string read_to_main_string(const std::string &main_string_file);
    static void remove_whitespace(std::string &str);
    static void to_lower(std::string &str);
    static Args& get_args(){return args;};
};


#endif //C_4_INITIALIZATION_H
