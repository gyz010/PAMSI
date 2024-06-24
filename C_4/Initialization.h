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
    Args() {
        algorithm = NAIVE;
        case_sensitivity = false;
        ignore_whitespace = false;

    }
};

class Initialization {
private:

public:
    static Args handle_args(int argc, char **argv);
    static std::string read_to_main_string(const std::string &main_string_file);
    static void remove_whitespace(std::string &main, std::string &sub);
    static void to_lower(std::string &main, std::string &sub);
};


#endif //C_4_INITIALIZATION_H
