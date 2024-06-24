cmake_minimum_required(VERSION 3.28)
project(C_4)

set(CMAKE_CXX_STANDARD 17)

add_executable(C_4 main.cpp
        StringSearch.cpp
        StringSearch.h
        Trie.cpp
        Trie.h
        Initialization.cpp
        Initialization.h
)
