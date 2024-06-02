//
// Created by gyz on 03.06.24.
//

#ifndef C3_3_EVALUATION_H
#define C3_3_EVALUATION_H

#include <vector>

namespace eval {
    class Evaluation {
    public:
        double evaluate(const std::vector<std::vector<int>> &board, bool is_black);
    };
}

#endif //C3_3_EVALUATION_H
