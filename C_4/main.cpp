#include <iostream>
#include "StringSearch.h"
#include "Initialization.h"
#include <algorithm>
#include <chrono>

/// Arguments: ./main [main_string_file] [sub_string] [algorithm] [bool:case_sensitivity] [bool:ignore_whitespace]

//todo przetestować dla jakiej liczby pierwszej algorytm rabin-karpa działa najszybciej (ma najmniejszą liczbę kolizji)

//todo dodac algorytm przeszukiwania drzewa koncowek

void run_with_args(Args &args) {
    using namespace std::chrono;
    std::string main = Initialization::read_to_main_string(args.main_string_file);
    if(args.ignore_whitespace) {
        Initialization::remove_whitespace(main, args.sub_string);
    }
    if(!args.case_sensitivity) {
        Initialization::to_lower(main, args.sub_string);
    }

    auto start = high_resolution_clock::now();
    size_t result = StringSearch::find(main, args.sub_string, args.algorithm);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cerr << "Time: " << duration.count() << " microseconds" << std::endl;
    std::cerr << "Result: " << result << std::endl;

    if(main.find(args.sub_string) != result) {
        std::cerr << "Error: result is incorrect" << std::endl;
    }
}

void auto_run() {
    using namespace std::chrono;
    std::string main = Initialization::read_to_main_string("../bible.txt");
    Algorithm algorithm = RABIN_KARP;

    for(size_t distance=1; distance-1<2'097'152; distance*=2) {
        std::cerr << "------------------------------------" << std::endl;
        std::cerr << "Distance: " << distance << std::endl;
        for(size_t length=1; length+distance-1<main.size(); length*=2) {
            std::string sub = main.substr(distance-1, length);
            auto start = high_resolution_clock::now();
            size_t result = StringSearch::find(main, sub, algorithm);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            if(main.find(sub) != result) {
                std::cerr << "Error: result is incorrect" << std::endl;
            }

            if(distance-1 == result) {
                std::cerr << "Time: " << duration.count() << " us, Length: " << length << std::endl;
            }

        }
    }
    size_t result;
}

void rk_collision_test() {
    using namespace std::chrono;
    std::string main = Initialization::read_to_main_string("../bible.txt");
//    std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83,
//                               89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
//                               181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251};
    std::vector<int> primes = {100000007};
    std::vector<size_t> collisions(primes.size(), 0);
    std::vector<long> times(primes.size(), 0);

    for(int i=0; i<primes.size(); i++) {
        for (size_t distance = 1; distance - 1 < 2'097'152; distance *= 2) {
            for (size_t length = 1; length + distance - 1 < main.size(); length *= 2) {
                std::string sub = main.substr(distance - 1, length);
                auto start = high_resolution_clock::now();
                size_t collision_buf = StringSearch::rabin_karp_collision_count(main, sub, primes[i]);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                times[i] += duration.count();
                if(collision_buf == -1) {
                    std::cerr << "Substring not found ???" << std::endl;
                }
                collisions[i] += collision_buf;
            }
        }
        std::cerr << "Prime: " << primes[i] << ", Collisions: " << collisions[i] << ", Time: " << times[i] << std::endl;
    }
}
int main(int argc, char **argv) {
    //Run with arguments
    if(argc==6) {
        try {
            Args args = Initialization::handle_args(argc, argv);
            run_with_args(args);
        } catch (std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
    //Autorun
    else {
        auto_run();
//        rk_collision_test();
    }
    return 0;
}
