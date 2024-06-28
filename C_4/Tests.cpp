//
// Created by gyz on 26.06.24.
//

#include "Tests.h"

void Tests::create_dna_sequence(std::string &main, size_t size) {
    std::random_device rand;
    main.reserve(size);
    for(int i=0; i<size; i++) {
        main += "ACGT"[rand() % 4];
    }
}

long Tests::dna_test(Algorithm algorithm) {
    using namespace std::chrono;
    std::string main;
    std::cout << "Creating DNA sequence..." << std::endl;
    create_dna_sequence(main, 160000);
    std::cout << "DNA sequence created" << std::endl;
    std::string sub = main.substr(159900, 100);

    long time = 0;

    if(algorithm != SUFFIX_ARRAY) {
        for(int i=0; i<100; i++) {
            auto start = high_resolution_clock::now();
            size_t result = StringSearch::find(main, sub, algorithm);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            time+=duration.count();
            if(result!=159900) {
                std::cerr << "Error: result is incorrect" << std::endl;
                return -1;
            }
        }
        time/=100;
        std::cerr << "Time: " << time/1000 << " microseconds" << std::endl;


    }
    else {
        auto start = high_resolution_clock::now();
        SuffixArray sa(main);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        std::cerr << "Building SA Time: " << duration.count()/1000000 << " milliseconds" << std::endl;
        for(int i=0; i<100; i++) {
            start = high_resolution_clock::now();
            size_t result = sa.find(sub);
            stop = high_resolution_clock::now();
            duration = duration_cast<nanoseconds>(stop - start);
            time+=duration.count();
            if(result!=159900) {
                std::cerr << "Error: result is incorrect" << std::endl;
                return -1;
            }
        }
        time/=100;
        std::cerr << "Time: " << time << " nanoseconds" << std::endl;

    }
    return time;
}

bool Tests::sub_length_and_sub_distance_test(Algorithm algorithm) {
    using namespace std::chrono;
    std::string text;
    try {
    text = Initialization::read_to_main_string("../test_files/test.txt"); ///////////////////////////////////////////////////ZMIENIĆ NA KONIEC
    } catch (std::invalid_argument &e) {
        std::cerr << "Brak pliku \"test.txt\" w katalogu głównym" << std::endl;
        return false;
    }
    SuffixArray *sa;
    size_t result;
    const int repetitions = 100;

    //excel stats
    int row=0;
    int col=0;
    std::vector<std::vector<long>> times;
    times.resize(50);
    static int excel_num=0;
    std::string path = "../time_complexity" + std::to_string(excel_num++) + ".csv";
    std::fstream excel_file(path, std::ios::out);


    for(size_t distance=1; distance-1<text.size(); distance*=2) {
        std::cerr << "------------------------------------" << std::endl;
        std::cerr << "Distance: " << distance << std::endl;
        for(size_t length=1; length+distance-1<text.size(); length*=2) {
            std::string main = text.substr(0, distance+length-1);
            std::string sub = main.substr(distance-1, length);
            if(algorithm == SUFFIX_ARRAY) {
                sa = new SuffixArray(main);
            }
            int64_t time = 0;
            for(int i=0; i<repetitions; i++) {
                if (algorithm != SUFFIX_ARRAY) {
                    auto start = high_resolution_clock::now();
                    result = StringSearch::find(main, sub, algorithm);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    time+=duration.count();
                } else {
                    auto start = high_resolution_clock::now();
                    result = sa->find(sub);
                    auto stop = high_resolution_clock::now();
                    auto duration = duration_cast<nanoseconds>(stop - start);
                    if (result != std::string::npos and main.substr(result, sub.size()) != sub) {
                        std::cerr << "Error: result is incorrect" << std::endl;
                        return false;
                    }
                    time+=duration.count();
                }
            }
            if (distance - 1 == result) {
                std::cerr << "Time: " << time/repetitions << " ns, Length: " << length << std::endl;
            }
            if(algorithm == SUFFIX_ARRAY) delete sa;

            //excel stats
            if(distance - 1 == result) {
                times[row].push_back(time / repetitions);
            } else {
                times[row].push_back(-1);
            }
            col++;
        }
        row++;
    }
    //excel stats
    for(auto & time : times) {
        for(long j : time) {
            excel_file << j << ",";
        }
        excel_file << std::endl;
    }
    excel_file.close();
    return true;
}

void Tests::rk_collision_test() {
    using namespace std::chrono;
    std::string main;
    std::cout << "Creating DNA sequence..." << std::endl;
    create_dna_sequence(main, 50000000);
    if(Initialization::get_args().case_sensitivity) {
        Initialization::to_lower(main);
    }
    if(Initialization::get_args().ignore_whitespace) {
        Initialization::remove_whitespace(main);
    }
    std::cout << "DNA sequence created" << std::endl;
    int prime;
    std::fstream prime_file("../primes.csv");
    std::fstream collision_file("../collisions.csv", std::ios::out);
    std::string sub = main.substr(40000000, 100); //100 znaków  zdecydowanie wystarczy żeby mieć pewność że będzie unikalne
    while(prime_file >> prime) {
        auto start = high_resolution_clock::now();
        size_t collisions = StringSearch::rabin_karp_collision_count(main, sub, prime);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        collision_file << prime << "," << collisions << "," << duration.count() << std::endl;
        std::cout << "Prime: " << prime << ", Collisions: " << collisions << ", Time: " << duration.count() << " microseconds" << std::endl;
    }
}

void Tests::perform_test(const Args& args) {
    if(args.test == "DNA") {
        if(!dna_test(args.algorithm)) {
            std::cerr << "DNA test failed" << std::endl;
        }
    }
    else if(args.test == "LENGTH_AND_DISTANCE") {
        if(!sub_length_and_sub_distance_test(args.algorithm)) {
            std::cerr << "Sub length and sub distance test failed" << std::endl;
        }
    }
    else if(args.test == "RK_COLLISION" and args.algorithm == RABIN_KARP) {
        rk_collision_test();
    }
    else {
        std::cerr << "Invalid test" << std::endl;
    }
}

void Tests::run_with_args(Args &args) {
    using namespace std::chrono;
    std::string main = Initialization::read_to_main_string(args.main_string_file);
    if(args.ignore_whitespace) {
        Initialization::remove_whitespace(main);
        Initialization::remove_whitespace(args.sub_string);
    }
    if(!args.case_sensitivity) {
        Initialization::to_lower(main);
        Initialization::to_lower(args.sub_string);
    }

    auto start = high_resolution_clock::now();
    size_t result = StringSearch::find(main, args.sub_string, args.algorithm);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    std::cerr << "Time: " << duration.count() << " microseconds" << std::endl;
    std::cerr << "Result: " << result << std::endl;
    if(args.algorithm != SUFFIX_ARRAY) {
        if (main.find(args.sub_string) != result) {
            std::cerr << "Error: result is incorrect" << std::endl;
        }
    }
    else {
        if (result != std::string::npos and main.substr(result, args.sub_string.size()) != args.sub_string) {
            std::cerr << "Error: result is incorrect" << std::endl;
        }
    }
}

void Tests::dna_variance_test(Algorithm algorithm) {
    const int size = 1000;

    std::vector<long> times;
    times.reserve(size);
    for(int i=0; i<size; i++) {
        times.push_back(dna_test(algorithm));
    }
    double mean = 0;
    for(auto &time : times) {
        mean+=time;
    }
    mean/=size;
    double variance = 0;
    for(auto &time : times) {
        variance+=pow(time-mean, 2);
    }
    variance/=size;
    std::cerr << "Mean: " << mean/1000 << " microseconds" << std::endl;
    std::cerr << "Standard deviation: " << sqrt(variance)/1000 << " microseconds" << std::endl;

}
