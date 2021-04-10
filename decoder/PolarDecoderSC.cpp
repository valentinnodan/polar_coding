//
// Created by valentinnodan on 11/5/20.
//

#include <cmath>
#include <polar_utils.h>
#include <chrono>
#include "PolarDecoderSC.h"

Message
PolarDecoderSC::decode(Message const &y, std::set<size_t> const &indices,
                       size_t N, size_t K) const {
    size_t n = y.size();
    size_t nn = log2(n);

    auto s = myChannel.getSigma(N, K);
    auto lN = Matrix<double>(n, nn + 1, NAN);
    auto uN = Matrix<double>(n, nn + 1, NAN);
    for (size_t i = 0; i < n; ++i) {
        lN[i][0] = 2 * y[i].symbol / s;
    }

    auto res = Message();
    res.reserve(n);
    Symbol currU{0};
    for (size_t i = 0; i < n; i++) {
        const double lNI = getLNRec(lN, uN, i, nn);
        if (indices.count(i) == 0) {
            currU = SymbolConsts::ZERO;
        } else {
            if (lNI >= 0) {
                currU = SymbolConsts::ZERO;
            } else {
                currU = SymbolConsts::ONE;
            }
        }
        res.push_back(currU);
        updateUNRec(uN, currU, i, nn);
    }
    return res;
}


double
PolarDecoderSC::getLNRec(Matrix<double> &lN, Matrix<double> const &uN, size_t i, size_t j) const {
    if (!isnanf(lN[i][j])) {
        return lN[i][j];
    }
    size_t upperNeighbourInd;
    size_t lowerNeighbourInd;
    size_t curr_i = i / 2;
    uint64_t step = curr_i % (1ul << (j - 1));
    upperNeighbourInd = 2 * curr_i - step;
    lowerNeighbourInd = (1ul << (j - 1)) + 2 * curr_i - step;
    double upper = getLNRec(lN, uN, upperNeighbourInd, j - 1);
    double lower = getLNRec(lN, uN, lowerNeighbourInd, j - 1);
    if (i % 2 == 0) {
        double d = (std::min(std::abs(lower), std::abs(upper)));
        lN[i][j] = (lower * upper) > 0 ? d : d * (-1);
    } else {
        lN[i][j] = upper * (1 - 2 * uN[2 * curr_i][j]) + lower;
    }

    return lN[i][j];
}

void PolarDecoderSC::updateUNRec(Matrix<double> &uN, Symbol bit, size_t ind, size_t s) const {
    if (s > 0) {
        uN[ind][s] = bit.symbol;
        size_t curr_i = ind / 2;
        uint64_t step = curr_i % (1ul << (s - 1));
        size_t upperNeighbourInd = 2 * curr_i - step;
        size_t lowerNeighbourInd = (1ul << (s - 1)) + 2 * curr_i - step;
        double upperBit;
        if (!isnanf(uN[2*curr_i][s]) && !isnanf(uN[2*curr_i + 1][s])) {
            upperBit = (int) (uN[2 * curr_i][s] + uN[2 * curr_i + 1][s]) % 2;
            updateUNRec(uN, Symbol(upperBit), upperNeighbourInd, s - 1);
        }
        double lowerBit = uN[2 * curr_i + 1][s];
        if (!isnanf(lowerBit)){
            updateUNRec(uN, Symbol(lowerBit), lowerNeighbourInd, s - 1);
        }
    }
}
