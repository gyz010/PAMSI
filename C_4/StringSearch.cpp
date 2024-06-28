//
// Created by gyz on 11.06.24.
//

#include "StringSearch.h"


size_t StringSearch::naive(const std::string &main, const std::string &sub) {
    size_t main_size = main.size();
    size_t sub_size = sub.size();

    for(size_t i = 0; i <= main_size - sub_size; i++) {
        size_t j = 0;
        while(j < sub_size && main[i + j] == sub[j]) {
            j++;
        }
        if(j == sub_size) {
            return i;
        }
    }
    return std::string::npos;
}

/// Rabin fingerprint
int64_t StringSearch::string_hash(const std::string &str, const size_t size, const int &alpha, const int &prime) {
    int64_t hash = 0;
    for (int i = 0; i < size; i++) {
        hash = (alpha * hash + str[i]) % prime;
    }
    return hash;

}

size_t StringSearch::rabin_karp(const std::string &main, const std::string &sub) {
    // Rozmiary stringów
    size_t main_size = main.size();
    size_t sub_size = sub.size();

    //Rozmiar alfabetu
    const int alpha = 128;
    const int prime = 100000007;
    int64_t sub_hash = string_hash(sub, sub_size, alpha, prime);
    int64_t main_hash = string_hash(main, sub_size, alpha, prime);

    // h = (alpha^(sub_size-1)) % prime
    int64_t h = 1;
    for (int i = 0; i < sub_size - 1; i++) {
        h = (h * alpha) % prime;
    }
    for (size_t i = 0; i <= main_size - sub_size; i++) {
        if (sub_hash == main_hash) {
            // Naiwny algorytm na wypadek kolizji
            size_t j = 0;
            while (j < sub_size && main[i + j] == sub[j]) {
                j++;
            }
            if (j == sub_size) {
                return i;
            }
        }
        // Obliczenie hashu dla kolejnego sub-stringa w głównym stringu
        if (i < main_size - sub_size) {
            main_hash = (alpha * (main_hash - main[i] * h) + main[i + sub_size]) % prime;
            if (main_hash < 0) {
                main_hash += prime;
            }
        }
    }
    return std::string::npos;
}

size_t StringSearch::rabin_karp_collision_count(const std::string &main, const std::string &sub, const int &prime) {
    size_t collisions = 0;
    // Rozmiary stringów
    size_t main_size = main.size();
    size_t sub_size = sub.size();

    //Rozmiar alfabetu
    const int alpha = 128;
    int64_t sub_hash = string_hash(sub, sub_size, alpha, prime);
    int64_t main_hash = string_hash(main, sub_size, alpha, prime);
    // h = (alpha^(sub_size-1)) % prime
    int64_t h = 1;
    for (int i = 0; i < sub_size - 1; i++) {
        h = (h * alpha) % prime;
    }
    for (size_t i = 0; i <= main_size - sub_size; i++) {
        if (sub_hash == main_hash) {
            // Naiwny algorytm na wypadek kolizji
            size_t j = 0;
            collisions++;
            while (j < sub_size && main[i + j] == sub[j]) {
                j++;
            }
            if (j == sub_size) {
                return collisions-1;
            }
        }
        // Obliczenie hashu dla kolejnego sub-stringa w głównym stringu
        if (i < main_size - sub_size) {
            main_hash = (alpha * (main_hash - main[i] * h) + main[i + sub_size]) % prime;
            if (main_hash < 0) {
                main_hash += prime;
            }
        }
    }
    return std::string::npos;;
}

size_t StringSearch::kmp(const std::string &main, const std::string &sub) {
    size_t main_size = main.size();
    size_t sub_size = sub.size();

    // Obliczanie tablicy prefiksowej
    std::vector<size_t> prefix(sub_size);
    prefix[0] = 0;
    size_t j = 0;
    for (size_t i = 1; i < sub_size; i++) {
        while (j > 0 && sub[j] != sub[i]) {
            j = prefix[j - 1];
        }
        if (sub[j] == sub[i]) {
            j++;
        }
        prefix[i] = j;
    }

    // Wyszukiwanie wzorca w tekście
    j = 0;
    for (size_t i = 0; i < main_size; i++) {
        while (j > 0 && sub[j] != main[i]) {
            j = prefix[j - 1];
        }
        if (sub[j] == main[i]) {
            j++;
        }
        if (j == sub_size) {
            return i - sub_size + 1;
        }
    }
    return std::string::npos;
}

size_t StringSearch::boyer_moore(const std::string &main, const std::string &sub) {
    size_t main_size = main.size();
    size_t sub_size = sub.size();

    // Obliczanie tablicy przesunięć
    std::vector<size_t> shift(256, sub_size);
    for (size_t i = 0; i < sub_size - 1; i++) {
        shift[sub[i]] = sub_size - i - 1;
    }

    // Wyszukiwanie wzorca w tekście
    size_t i = sub_size - 1;
    while (i < main_size) {
        int64_t j = sub_size - 1;
        while (j >= 0 && main[i] == sub[j]) {
            i--;
            j--;
        }
        if (j == -1) {
            return i + 1;
        }
        i += std::max(shift[main[i]], sub_size - j);
    }
    return std::string::npos;
}

size_t StringSearch::find(const std::string &main, const std::string &sub, Algorithm algorithm) {
    switch (algorithm) {
        case NAIVE:
            return naive(main, sub);
        case RABIN_KARP:
            return rabin_karp(main, sub);
        case KMP:
            return kmp(main, sub);
        case BOYER_MOORE:
            return boyer_moore(main, sub);
        case SUFFIX_ARRAY:
            SuffixArray suffixArray(main);
            return suffixArray.find(sub);


    }
}




