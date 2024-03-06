//
// Created by Asus on 05.03.2024.
//

#ifndef C1_1_STACK_H
#define C1_1_STACK_H

#include <memory>
#include <iostream>


template <typename T, typename Alloc=std::allocator<T>>
class stack_array {
private:
    T* base;
    T* end;
    T* top;
    Alloc alloc;

private:
    void double_size() {
        size_t size = end-base;
        try {
            if(base==end) {
                T *temp_base = alloc.allocate(1, base);
                top = temp_base;
                base = temp_base;
                end = temp_base + 1;
            }
            else {
                T *temp_base = alloc.allocate((size) << 1, base);
                std::uninitialized_move(base, end, temp_base);
                // Deallocate previous memory
                for (size_t i = 0; i < size; i++) {
                    alloc.destroy(&base[i]);
                }
                alloc.deallocate(base, size);
                top = temp_base + (top - base);
                base = temp_base;
                end = temp_base + ((size) << 1);
            }
        }
        catch (std::bad_alloc &e) {
            std::cout << e.what() << std::endl;
        }
    }


public:
    stack_array() {
        end=top=base;
    }
    ~stack_array() {
        size_t size = end-base;
        for (int i = 0; i < size; i++) {
            alloc.destroy(&base[i]);
        }
    }

    void clear() {
        size_t size = end-base;
        for (size_t i = 0; i < size; i++) {
            alloc.destroy(&base[i]);
        }
        alloc.deallocate(base, size);
        end=top=base;
    }

    void push(const T &data) {
        if(top == end) {
            double_size();
        }
        *top=data;
        top++;
    }
    void emplace(const T &data) {
        if(top == end) {
            double_size();
        }
        *top = std::move(data);
        top++;
    }
    T pop() {
        if(empty()){
            throw (std::runtime_error("Stack is empty"));
        }
         top--;
         return *top;
    }
    bool empty() {
        return top == base;
    }

    void reserve(const size_t &size) {

        T *temp_base = alloc.allocate(size, base);
        if(end-base != 0) {
            std::uninitialized_move(base, base + std::min(size, static_cast<size_t>(end - base)), temp_base);
            // Deallocate previous memory
            for (int i = 0; i < size; i++) {
                alloc.destroy(&base[i]);
            }

            alloc.deallocate(base, std::min(size, static_cast<size_t>(end - base)));
        }

        top = temp_base + (top - base);
        base = temp_base;
        end = temp_base + size;

        // Reserved memory is less than current
        if(top>end) {
            top=end;
        }
    }

    size_t size() {
        return top-base;
    }
    size_t capacity() {
        return end-base;
    }
    T peek() {
        if(empty()){
            throw (std::runtime_error("Stack is empty"));
        }
        return top[-1];
    }

};

#endif //C1_1_STACK_H
