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
#include "PolarDecoder.h"

class PolarDecoderSC : PolarDecoder {
public:
    explicit PolarDecoderSC(Channel const &m, size_t n, size_t k) : PolarDecoder(m, n, k) {
    }

    Message decode(Message const &y, std::set<size_t> const &indices);


private:

    void updateUNRec(Matrix<double> & uN, Symbol bit, size_t ind, size_t s) const;

    double
    getLNRec(Matrix<double> &lN, const Matrix<double> &uN, size_t i, size_t j) const;
};

