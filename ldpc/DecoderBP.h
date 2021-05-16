//
// Created by valentinnodan on 3/21/21.
//

#pragma once


#include <Symbol.h>
#include <Matrix.h>
#include <unordered_set>
#include <set>

class DecoderBP {

    double f(double x) const;

    size_t n;
    size_t r;

public:

    DecoderBP(size_t n, size_t r) : n(n), r(r) {}

    static std::pair<Matrix<size_t>, Matrix<size_t>> prepare(Matrix<int> const &h);

    Message decode(size_t needed, std::vector<double> const &y,
                   Matrix<size_t> const &c,
                   Matrix<size_t> const &v,
                   size_t iter = 10) const;
};
