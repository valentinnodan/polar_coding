//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <vector>
#include <set>
#include "Matrix.h"
#include <cmath>


//did not count B_N in encoding to simplify the implementation
class PolarCoder {
public:
    static std::vector<uint64_t>
    encode(std::vector<uint64_t> const &u, std::set<size_t> const &indices, std::vector<uint64_t> const &frozen);

private:
    const static Matrix<uint64_t> F;

    static Matrix<uint64_t> getGN(int n);
};
