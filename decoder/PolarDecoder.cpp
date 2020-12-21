//
// Created by valentinnodan on 11/5/20.
//

#include <cmath>
#include <polar_utils.h>
#include <iostream>
#include "PolarDecoder.h"

Message
PolarDecoder::decode(Message const &y, std::set<size_t> const &indices,
                     Message const &frozen, std::vector<size_t> const &reversedIndexes, size_t N, size_t K) const {
    size_t n = y.size();
    size_t nn = log2(n);

    auto revPlace = std::vector<size_t>(n);
    for (size_t i = 0; i < n; i++) {
        revPlace[reversedIndexes[i]] = i;
    }

    auto s = myChannel.getSigma(N, K);
    auto lN = Matrix<double>(n, nn + 1, NAN);
    for (size_t i = 0; i < n; ++i) {
        lN[i][nn] = 2 * y[i].symbol / s;
    }

    auto res = Message(y.size());
    size_t currFrozenInd = 0;
    Symbol currU{0};
    for (size_t i = 0; i < y.size(); i++) {
        currU = h(res, i, lN, revPlace[i]);
        if (indices.count(i) == 0) {
            currU = frozen[currFrozenInd++];
        }
        res[i] = currU;
    }
    return res;
}

Symbol PolarDecoder::h(Message const &u, size_t uLength, Matrix<double> &lN, size_t placeL) const {
    const double lNI = getLNRec(lN, placeL, 0, u, uLength);
    if (lNI >= 0) {
        return SymbolConsts::ZERO;
    }
    return SymbolConsts::ONE;
}


double PolarDecoder::getLNRec(Matrix<double> &lN, size_t i, size_t j, Message const &u, size_t uLength) const {
    if (!isnanf(lN[i][j]))  {
        return lN[i][j];
    }
    size_t upperNeighbourInd, lowerNeighbourInd;
    uint64_t step = lN.height / (1ul << (j + 1));
    if (i % (step * 2) >= step) {
        upperNeighbourInd = i - step;
        lowerNeighbourInd = i;
    } else {
        upperNeighbourInd = i;
        lowerNeighbourInd = i + step;
    }
    auto pairMessages = getOddsEvens(u, uLength);
    Message neighbourU, otherNeighbourU;
    double upper = getLNRec(lN, upperNeighbourInd, j + 1, pairMessages.first, pairMessages.first.size());
    double lower = getLNRec(lN, lowerNeighbourInd, j + 1, pairMessages.second, pairMessages.second.size());
    if (uLength % 2 == 0) {
        double d = (std::min(std::abs(lower), std::abs(upper)));
        lN[i][j] = (lower * upper) > 0 ? d : d * (-1);
    } else {
        lN[i][j] = upper * (1 - 2 * u[uLength - 1].symbol) + lower;
    }

    return lN[i][j];
}
