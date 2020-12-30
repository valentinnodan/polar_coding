//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <vector>
#include <cstdint>
#include <set>
#include <Matrix.h>

#include "Symbol.h"
#include "../channel/Channel.h"

class PolarDecoder {
public:
    explicit PolarDecoder(Channel const &m) : myChannel(m) {
    }

    Message decode(Message const &y, std::set<size_t> const &indices, Message const &frozen,
                   std::vector<size_t> const &reversedIndexes, size_t N, size_t K) const;

private:

    Channel myChannel;

    void updateUNRec(Matrix<double> & uN, Symbol bit, size_t ind, size_t s) const;

    double
    getLNRec(Matrix<double> &lN, const Matrix<double> &uN, size_t i, size_t j) const;
};

