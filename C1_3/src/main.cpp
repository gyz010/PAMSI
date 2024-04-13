#include <iostream>
#include "../inc/mod.h"
#include <fstream>

typedef struct arguments {
    std::string filename;
    size_t offset;
    size_t msg_size;
    size_t packet_size;
    arguments() : offset(0), msg_size(0), packet_size(0){};
}arguments;

size_t getFileLength(const std::string &filename) {
    std::ifstream file(filename);
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.close();
    return length;
}

arguments flag_handle(const int &argc, char **argv) {
// Nazwa pliku, offset, msg_size, pack_size
    arguments args;
    if(argc != 5) {
        throw std::invalid_argument("[nazwa_pliku] [offset] [msg_size] [packet_size]");
    }
    args.filename = argv[1];
    if(std::stoll(argv[2]) >= 0) {
        args.offset = std::stoll(argv[2]);
    }
    else throw (std::invalid_argument("Offset nie moze byc ujemny"));

    if(std::stoll(argv[3]) >= 0) {
        args.msg_size = std::min(static_cast<uint64_t>(std::stoll(argv[3])), getFileLength(args.filename));
    }
    else throw (std::invalid_argument("Rozmiar wiadomosci nie moze byc ujemny"));

    if(std::stoll(argv[4]) > 0) {
        args.packet_size = std::stoll(argv[4]);
    }
    else throw (std::invalid_argument("Rozmiar pakietu musi byc dodatni"));
    return args;
}

std::string* read_to_array(const arguments &args) {
    std::ifstream file(args.filename);
    if(!file.is_open()) {
        std::cerr << "Blad otwierania pliku" << std::endl;
        return nullptr;
    }
    std::string* packets;
    size_t packet_count = std::ceil(static_cast<double>(args.msg_size)/static_cast<double>(args.packet_size));
    packets = new std::string[packet_count];

    file.seekg(args.offset);
    std::string packet;
    packet.resize(args.packet_size);
    size_t count=0;
    char c;
    while(!file.eof() and count != packet_count) {
        for(size_t p = 0; p < args.packet_size; p++) {
            file.get(c);
            packet[p] = c;

            //Last packet edge case
            if(file.eof()) {
                packet.resize(p+1);
                break;
            }
        }
        packets[count]=packet;
        count++;
    }
    file.close();
    return packets;
}

//funckja użyta do przeprowadzenia statystyk w raporcie

/*void tests_and_stats() {
    using namespace std::chrono;
    arguments args;
    args.filename="../bible.txt";
    args.packet_size=10;
    std::ofstream file("../stats.csv", std::ios::out);
    packet_assembler assembler;
    int* random_order;
    duration<long, std::ratio<1, 1000000000>> insert_time[100];
    duration<long, std::ratio<1, 1000000000>> printout_time[100];
    for(size_t message_size=8; message_size<200000000; message_size*=2) {
        args.msg_size=message_size;
        size_t packet_count = std::ceil(static_cast<double>(args.msg_size)/static_cast<double>(args.packet_size));
        random_order=packet_assembler::create_random_order_array(packet_count);
        for(int i=0; i<100; i++) {
            std::string* packets = read_to_array(args);
            auto start = high_resolution_clock::now();
            assembler.insert_packets(packets, packet_count, random_order);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<std::chrono::nanoseconds>(end - start);
            insert_time[i] = duration;

            start = high_resolution_clock::now();
            assembler.in_order_printout();
            end = high_resolution_clock::now();
            duration = duration_cast<std::chrono::nanoseconds>(end - start);
            printout_time[i] = duration;

            assembler.clear();
            delete[] packets;
        }
        delete[] random_order;
        duration<long, std::ratio<1, 1000000000>> insert_mean = std::chrono::nanoseconds (0) ;
        for(auto i : insert_time) {
            insert_mean+=i;
        }
        duration<long, std::ratio<1, 1000000000>> printout_mean = std::chrono::nanoseconds (0) ;
        for(auto i : printout_time) {
            printout_mean+=i;
        }
        insert_mean/=100;
        printout_mean/=100;

        file << packet_count <<","<< insert_mean.count() <<","<<printout_mean.count()<<","<<std::endl;
    }
    file.close();
}*/

int main(int argc, char** argv) {
    arguments args;
    try {
        args = flag_handle(argc, argv);
    }
    catch (std::invalid_argument &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    size_t packet_count = std::ceil(static_cast<double>(args.msg_size)/static_cast<double>(args.packet_size));
    std::string *packets = read_to_array(args);
    if(packets == nullptr) {
        return -1;
    }
    packet_assembler assembler;
    assembler.shuffle_and_insert_packets(packets, packet_count);
    assembler.in_order_printout();
    assembler.clear();
    delete[] packets;
    return 0;
}
