//
// Created by valentinnodan on 3/21/21.
//

#pragma once

#include <Matrix.h>
#include <set>
#include <cmath>
#include "coder/PolarCoder.h"

inline Matrix<int> transform(size_t n, const std::set<size_t> &indices) {
    auto nn = log2(n);
    auto H = Matrix<int>(n * (nn + 1), n * nn);
    size_t cNInd = 0;
    for (size_t i = 0; i < nn; i++) {
        auto flags = std::vector<bool>(n, false);
        size_t ii = i + 1;
        size_t step = n / (1 << ii);
        for (size_t j = 0; j < n; j++) {
            if (!flags[j]) {
                flags[j] = true;
                flags[j + step] = true;

                size_t vN1 = j + (ii - 1) * n;
                size_t vN2 = j + step + (ii - 1) * n;
                H[vN1][cNInd] = 1;
                H[vN2][cNInd] = 1;
                H[vN1 + n][cNInd] = 1;
                cNInd += 1;
                H[vN2][cNInd] = 1;
                H[vN2 + n][cNInd] = 1;
                cNInd += 1;
            }
        }
    }
    // sub size of indices
//    std::cout << indices.size() << std::endl;
//    auto resH = Matrix<int>(n * (nn + 1), indices.size());
//    size_t ind = 0;
//    for (size_t i = 0; i < n; i++) {
//        if (indices.count(i) == 0) {
//            continue;
//        }
//        for (size_t j = 0; j < n * (nn + 1); j++) {
//            resH[j][ind] = H[j][i];
//        }
//    }
//    std::cout << resH;
    return H;
}

inline Matrix<int> transformNaive(size_t n, const std::set<size_t> &indices) {
    auto H = Matrix<int>(n - indices.size(), n);
    auto G = PolarCoder::getGN(n);
    size_t ind;
    for (size_t i = 0; i < n; i++) {
        ind = 0;
        for (size_t j = 0; j < n; j++) {
            if (indices.count(j) > 0) {
                continue;
            }
            H[ind][i] = G[ind][i].symbol;
            ind++;
        }
    }
    return H;
}