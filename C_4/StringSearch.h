//
// Created by gyz on 11.06.24.
//

#ifndef C_4_STRINGSEARCH_H
#define C_4_STRINGSEARCH_H


#include <string>
#include <vector>
#include <iostream>
#include "SuffixArray.h"

enum Algorithm {
    NAIVE,
    RABIN_KARP,
    KMP,
    BOYER_MOORE,
    SUFFIX_ARRAY
};

class StringSearch {
private:
    static int64_t string_hash(const std::string &str, size_t size, const int &alpha, const int &prime);

    static size_t naive(const std::string &main, const std::string &sub);
    static size_t rabin_karp(const std::string &main, const std::string &sub);
    static size_t kmp(const std::string &main, const std::string &sub);
    static size_t boyer_moore(const std::string &main, const std::string &sub);
public:
    static size_t find(const std::string &main, const std::string &sub, Algorithm algorithm);
    static size_t rabin_karp_collision_count(const std::string &main, const std::string &sub, const int &prime);
};


#endif //C_4_STRINGSEARCH_H
