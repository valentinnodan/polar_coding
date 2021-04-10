//
// Created by valentinnodan on 11/5/20.
//

#include <iostream>
#include <polar_utils.h>

#include "PolarCoder.h"

Matrix<Symbol> PolarCoder::getFN(int n) {
    auto myRes = F;
    for (int i = 0; i < n - 1; i++) {
        myRes = F * myRes;
    }
    return myRes;
}

Message PolarCoder::encode(Message const &u, std::set<size_t> const &indices,
                           Message const &frozen, std::vector<size_t> const & reversedIndexes,
                           bool reverse) {
    size_t n = indices.size() + frozen.size();
    size_t nn = log2(n);
    Message word;
    if (u.size() == n) {
        for (size_t i = 0; i < n; i++) {
            word.emplace_back(u[i].symbol);
        }
    } else {
        word = getWord(indices, frozen, u);
    }
    for (size_t i = 0; i < nn; i++) {
        auto flags = std::vector<bool>(n, false);
        size_t ii = i + 1;
        size_t step = n / (1ul << ii);
        for (size_t j = 0; j < n; j++) {
            if (!flags[j]) {
                word[j].symbol = static_cast<int>(word[j].symbol + word[j + step].symbol) % 2;
                flags[j] = true;
                flags[j + step] = true;
            }
        }
    }
    if (reverse) {
        auto codeWordRes = Message(n);
        for (size_t i = 0; i < n; i++) {
            codeWordRes[i] = word[reversedIndexes[i]];
        }
        return codeWordRes;
    }
    return word;
}

Matrix<Symbol> PolarCoder::RN(Matrix<Symbol> const &m) {
    auto res = Matrix<Symbol>(m.height, m.width);
    for (size_t i = 0; i < m.height; ++i) {
        for (size_t j = 0; j < m.width; ++j) {
            if (j < m.width / 2) {
                res[i][j] = m[i][2 * j];
            } else {
                res[i][j] = m[i][(j - m.width / 2) * 2 + 1];
            }
        }
    }
    return res;
}

Matrix<Symbol> PolarCoder::getBN(int n) {
    if (n == 2) {
        return I2;
    }
    auto B = getBN(n / 2);
    auto multRes = I2 * B;
    auto r = RN(multRes);
    return r;
}

Matrix<Symbol> PolarCoder::getGN(int n) {
    size_t nn = log2(n);
    auto fN = getFN(nn);
    auto BN = getBN(n);
    return Matrix<Symbol>::mult(BN, fN);
}

Message PolarCoder::getWord(std::set<size_t> const & A, Message const & frozen, Message const & u) {
    size_t myMsgInd = 0;
    size_t n = u.size() + frozen.size();
    auto word = Message(n);
    for (size_t i = 0; i < n; ++i) {
        if (A.count(i) == 1) {
            word[i].symbol = u[myMsgInd++].symbol;
        } else {
            word[i].symbol = Symbol(0).symbol;
        }
    }
    return word;
}