//
// Created by gyz on 26.06.24.
//

#ifndef C_4_TESTS_H
#define C_4_TESTS_H

#include "StringSearch.h"
#include "Initialization.h"
#include <chrono>
#include <random>

class Tests {
private:
    static void create_dna_sequence(std::string &main, size_t size);
public:
    static long dna_test(Algorithm algorithm);
    static bool sub_length_and_sub_distance_test(Algorithm algorithm);
    static void rk_collision_test();
    static void perform_test(const Args& args);
    static void run_with_args(Args& args);
    static void dna_variance_test(Algorithm algorithm);
};


#endif //C_4_TESTS_H
