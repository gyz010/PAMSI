#include <iostream>
#include "Tests.h"
#include <algorithm>
#include <chrono>
#include "SuffixArray.h"

/// Arguments: ./main [main_string_file] [sub_string] [algorithm] [bool:case_sensitivity] [bool:ignore_whitespace]

//todo przetestować dla jakiej liczby pierwszej algorytm rabin-karpa działa najszybciej (ma najmniejszą liczbę kolizji)

//todo dodac algorytm przeszukiwania drzewa koncowek

//dna test - algorytm BM ma dużą wariację.



void suffix_array_test() {
    using namespace std::chrono;
    std::string main = Initialization::read_to_main_string("../bible.txt");
    std::string pattern = " of";
    Initialization::remove_whitespace(main);
    Initialization::remove_whitespace(pattern);
    auto start = high_resolution_clock::now();
    SuffixArray sa(main);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cerr << "Time: " << duration.count() << " microseconds" << std::endl;

    start = high_resolution_clock::now();
    size_t result = sa.find(pattern);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    std::cerr << "Time: " << duration.count() << " microseconds" << std::endl;
    std::cerr << "Result: " << result << std::endl;
    auto r = main.find(pattern);
    if(r == std::string::npos) {
        std::cerr << "Substring not found" << std::endl;
    }
    else if(r != result) {
        std::cerr << "Error: result is incorrect" << std::endl;
        std::cerr << "Result: " << result << ", Correct: " << r << std::endl;
    }
}

int main(int argc, char **argv) {
//    Run with arguments
    if(argc==6 || argc==5) {
        try {
            Initialization::handle_args(argc, argv);
            if(argc == 6) Tests::run_with_args(Initialization::get_args());
            if(argc == 5) Tests::perform_test(Initialization::get_args());
        } catch (std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
    //Autorun
    else {
        Tests::sub_length_and_sub_distance_test(RABIN_KARP);
        Tests::sub_length_and_sub_distance_test(KMP);
        Tests::sub_length_and_sub_distance_test(BOYER_MOORE);
        Tests::sub_length_and_sub_distance_test(SUFFIX_ARRAY);
//        rk_collision_test();
//        suffix_array_test();
//        Tests::dna_test(SUFFIX_ARRAY);
//        Tests::dna_variance_test(SUFFIX_ARRAY);
//          Tests::rk_collision_test();
    }
    return 0;
}
