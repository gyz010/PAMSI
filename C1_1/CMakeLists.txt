cmake_minimum_required(VERSION 3.26)
project(C1_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(C1_1 src/main.cpp
        inc/stack_array.h
        inc/stack_list.h)
