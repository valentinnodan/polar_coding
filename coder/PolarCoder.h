//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <vector>
#include <set>
#include "Matrix.h"
#include <cmath>


class PolarCoder {
public:
    static std::vector<uint64_t>
    encode(std::vector<uint64_t> const &u, std::set<size_t> const &indices, std::vector<uint64_t> const &frozen);

private:
    const static Matrix<uint64_t> F;
    const static Matrix<uint64_t> I2;

    static Matrix<uint64_t> getFN(int n);
    static Matrix<uint64_t> RN(Matrix<uint64_t> m);
    static Matrix<uint64_t> getBN(int n);
    static Matrix<uint64_t> getGN(int n);
};
