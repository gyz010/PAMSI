#include <iostream>
#include <vector>
#include <chrono>
#include "../inc/stack_array.h"
#include "../inc/stack_list.h"

void stack_array_push_dynamic_test(const size_t &size) {
    stack_array<int> s_arr;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<size; i++) {
        s_arr.push(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "stack_array with dynamic allocation, size = " << size << ": ";
    std::cout << static_cast<double>(duration.count())/1000.f << "ms" << std::endl;
}
void stack_array_push_reserve_test(const size_t &size) {
    stack_array<int> s_arr;
    auto start = std::chrono::high_resolution_clock::now();
    s_arr.reserve(size);
    for(int i=0; i<size; i++) {
        s_arr.push(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "stack_array with memory reservation, size = " << size << ": ";
    std::cout << static_cast<double>(duration.count())/1000.f << "ms" << std::endl;
}

void stack_list_push_test(const size_t &size) {
    stack_list<int> s_list;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<size; i++) {
        s_list.push(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "stack_list:, size = " << size <<": " ;
    std::cout << static_cast<double>(duration.count())/1000.f << "ms" << std::endl;
}

void push_test() {
    stack_array_push_reserve_test(100);
    stack_array_push_dynamic_test(100);
    stack_list_push_test(100);
    std::cout << std::endl;
    stack_array_push_reserve_test(1000);
    stack_array_push_dynamic_test(1000);
    stack_list_push_test(1000);
    std::cout << std::endl;
    stack_array_push_reserve_test(10000);
    stack_array_push_dynamic_test(10000);
    stack_list_push_test(10000);
    std::cout << std::endl;
}

int main() {
    push_test();
}
