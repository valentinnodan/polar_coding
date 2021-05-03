//
// Created by valentinnodan on 4/12/21.
//

#pragma once

#include "PolarDecoder.h"
#include <Matrix.h>

class PolarDecoderLP : PolarDecoder{

public:
    explicit PolarDecoderLP(Channel const &m, std::pair<Matrix<size_t>,
    Matrix<size_t>> const &pair) : PolarDecoder(m), pair(pair) {
    }
    double alpha = 0.1;
    Message decode(Message const &y, std::set<size_t> const &indices, size_t N, size_t K) const;

private:
    std::pair<Matrix<size_t>,
    Matrix<size_t>> const &pair;
};
