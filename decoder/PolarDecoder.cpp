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
    auto lN = Matrix<double>(n, nn + 1, -1);
    for (size_t i = 0; i < n; ++i) {
        lN[i][nn] = 2 * y[i].symbol / s;
    }

    auto res = Message(y.size());
    size_t currFrozenInd = 0;
    for (size_t i = 0; i < y.size(); i++) {
        Symbol currU{0};
        if (indices.count(i) == 0) {
            h(res, i, lN, revPlace[i]);
            currU = frozen[currFrozenInd++];
            res[i] = currU;
        } else {
            currU = h(res, i, lN, revPlace[i]);
            res[i] = currU;
        }
    }
//    for (size_t i = 0 ; i< n; ++i) {
//        for (size_t j = 0; j < nn + 1; ++j) {
//            std::cout << "("<< lN[i][j].second  << ", ";
//            for (Symbol c: lN[i][j].first) {
//                std::cout << c;
//            }
//            std::cout << ")      ";
//        }
//        std::cout << std::endl;
//    }
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
    if (j < lN.width - 1) {
        int sign = 1;
        uint64_t step = lN.height / (1ul << (j + 1));
        if (i % (step * 2) >= step) {
            sign = -1;
        }
        double neighbour = lN[i][j + 1];
        double otherNeighbour = lN[i + sign * step][j + 1];
        auto pairMessages = getOddsEvens(u, uLength);
        Message neighbourU, otherNeighbourU;
        if (sign > 0) {
            otherNeighbourU = pairMessages.second;
            neighbourU = pairMessages.first;
        } else {
            otherNeighbourU = pairMessages.first;
            neighbourU = pairMessages.second;
        }
        if (neighbour == -1) {
            neighbour = getLNRec(lN, i, j + 1, neighbourU, neighbourU.size());
        }
        if (otherNeighbour == -1) {
            otherNeighbour = getLNRec(lN, i + sign * step, j + 1, otherNeighbourU, otherNeighbourU.size());
        }
        if (uLength % 2 == 0) {
            double d = (std::min(std::abs(neighbour), std::abs(otherNeighbour)));
            lN[i][j] = (neighbour * otherNeighbour) > 0 ? d : d * (-1);
        } else {
            if (sign > 0) {
                lN[i][j] = neighbour * (1 - 2 * u[uLength - 1].symbol) + otherNeighbour;
            } else {
                lN[i][j] = otherNeighbour * (1 - 2 * u[uLength - 1].symbol) + neighbour;
            }
        }
        return lN[i][j];
    } else {
        return lN[i][j];
    }
}
