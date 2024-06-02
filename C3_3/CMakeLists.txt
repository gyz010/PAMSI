cmake_minimum_required(VERSION 3.28)
project(C3_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(C3_3 src/main.cpp
        src/Checkers.cpp
        inc/Checkers.h
        src/Bot.cpp
        inc/Bot.h
        src/BoardActions.cpp
        inc/BoardActions.h
        src/Evaluation.cpp
        inc/Evaluation.h
)
