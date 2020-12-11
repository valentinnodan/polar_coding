//
// Created by valentinnodan on 11/5/20.
//

#include <cmath>
#include <polar_utils.h>
#include <iostream>
#include "PolarDecoder.h"

Message
PolarDecoder::decode(Message const &y, std::set<size_t> const &indices,
                     Message const &frozen, size_t N, size_t K) const {
    size_t currFrozenInd = 0;
    size_t n = y.size();
    size_t nn = log2(n);
    auto lN = Matrix<std::pair<Message, double>>(n, nn + 1, std::pair<Message, double>(Message(), -1));

    for (size_t i = 0; i < n; ++i) {
        auto w0 = myChannel.getW(y[i], SymbolConsts::ZERO, N, K);
        auto w1 = myChannel.getW(y[i], SymbolConsts::ONE, N, K);
        lN[i][nn] = std::pair<Message, double>(Message(),  w0 - w1);
    }

    auto uI = Message();
    auto res = Message(y.size());
    for (size_t i = 0; i < y.size(); i++) {
        Symbol currU{0};
        if (indices.count(i) == 0) {
            getLN(y, uI, lN);
            currU = frozen[currFrozenInd++];
            res[i] = currU;
        } else {
            currU = h(y, uI, lN);
            res[i] = currU;
        }
        uI.push_back(currU);
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

Symbol PolarDecoder::h(Message const &y, Message const &u, Matrix<std::pair<Message, double>> &lN) const {
    const double lNI = getLN(y, u, lN);
    if (lNI >= 0) {
        return SymbolConsts::ZERO;
    }
    return SymbolConsts::ONE;
}

double PolarDecoder::getLN(Message const &y, Message const &u, Matrix<std::pair<Message, double>> &lN) const {
    size_t u_i = u.size();
    auto t = genReversedIndex(y.size());
    size_t myInd = 0;
    for (size_t i = 0; i < y.size(); ++i) {
        if (t[i] == u_i) {
            myInd = i;
            break;
        }
    }
    return getLNRec(lN, myInd, 0, u);
}

double PolarDecoder::getLNRec(Matrix<std::pair<Message, double>> &lN, size_t i, size_t j, Message const & u) const {
    if (j < lN.width - 1) {
        int sign = 1;
        uint64_t step = lN.height / (1 << (j + 1));
        if (i % (step * 2) >= step) {
            sign = -1;
        }
        lN[i][j].first = u;
        std::pair<Message, double> neighbour = lN[i][j + 1];
        std::pair<Message, double> otherNeighbour = lN[i + sign * step][j + 1];
        auto pairMessages = getOddsEvens(u);
        if (sign > 0) {
            otherNeighbour.first = pairMessages.second;
            neighbour.first = pairMessages.first;
        } else {
            otherNeighbour.first = pairMessages.first;
            neighbour.first = pairMessages.second;
        }
        if (neighbour.second == -1) {
            neighbour.second = getLNRec(lN, i, j + 1, neighbour.first);
        }
        if (otherNeighbour.second == -1) {
            otherNeighbour.second = getLNRec(lN, i + sign * step, j + 1, otherNeighbour.first);
        }
        if (lN[i][j].first.size() % 2 == 0) {
            double d = (std::min(std::abs(neighbour.second), std::abs(otherNeighbour.second)));
            lN[i][j].second = (neighbour.second * otherNeighbour.second) > 0 ? d : d * (-1);
        } else {
            if (sign > 0) {
                lN[i][j].second = neighbour.second * (1 - 2 * u[u.size() - 1].symbol) + otherNeighbour.second;
            } else {
                lN[i][j].second = otherNeighbour.second * (1 - 2 * u[u.size() - 1].symbol) + neighbour.second;
            }
        }
        return lN[i][j].second;
    } else {
        return lN[i][j].second;
    }
}
