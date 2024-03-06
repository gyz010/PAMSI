#include <iostream>
#include <vector>
#include <chrono>
#include "../inc/stack_array.h"
#include "../inc/stack_list.h"

int main() {

    stack_list<int> s;

    for(int i=0; i<1000; i++) {
        s.push(i);
    }
    for(int i=0; i<1000; i++) {
        std::cout << s.peek() << std::endl;
        s.pop();
    }




    /*typedef struct complexDataType {
        int data1;
        float data2;
        char data3;
        size_t data4;
        bool data5;
    };
    complexDataType a={1, 2.5, 'c', 125364609, false};

    stack_array<complexDataType> s;
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;


    s.reserve(100000);



    start = std::chrono::system_clock::now();
    for(int i=0; i<100000; i++) {
        s.emplace({1, 2.5, 'c', 125364609, false});
    }
    end = std::chrono::system_clock::now();
    elapsed = end - start;
    std::cout << "Emplace: " << elapsed.count() << '\n';

    s.clear();


    s.reserve(100000);


    start = std::chrono::system_clock::now();
    for(int i=0; i<100000; i++) {
        s.push({1, 2.5, 'c', 125364609, false});
    }
    s.clear();

    end = std::chrono::system_clock::now();
    elapsed = end - start;
    std::cout << "Push: " << elapsed.count() << '\n';*/
    return 0;
}
