//
// Created by valentinnodan on 11/5/20.
//

#include <iostream>

#include "PolarCoder.h"

Matrix<Symbol> PolarCoder::getFN(int n) {
    auto myRes = F;
    for (int i = 0; i < n - 1; i++) {
        myRes = F * myRes;
    }
    return myRes;
}

Message PolarCoder::encode(Message const &u, std::set<size_t> const &indices,
                           Message const &frozen, std::vector<size_t> const & reversedIndexes) {
    size_t n = indices.size() + frozen.size();
    size_t nn = log2(n) + 1;
    auto word = getWord(indices, frozen, u);
    auto codingMatrix = Matrix<Symbol>(n, nn);
    for (size_t i = 0; i < n; i++) {
        codingMatrix[i][0] = word[i];
    }
    for (size_t i = 1; i < nn; i++) {
        for (size_t j = 0; j < n; j++) {
            size_t step = n / (1ull << (i));
            if (j % (step * 2) >= step) {
                codingMatrix[j][i] = codingMatrix[j][i - 1];
            } else {
                codingMatrix[j][i].symbol = (
                        (int) (codingMatrix[j][i - 1].symbol + codingMatrix[j + step][i - 1].symbol) % 2);
            }
        }
    }
    auto codeWordRes = Message(n);
    for (size_t i = 0; i < n; i++) {
        codeWordRes[i] = codingMatrix[reversedIndexes[i]][nn - 1];
    }
    return codeWordRes;
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
    size_t frozenInd = 0;
    size_t n = u.size() + frozen.size();
    auto word = Message(n);
    for (size_t i = 0; i < n; ++i) {
        if (A.count(i) == 1) {
            word[i] = u[myMsgInd];
        } else {
            word[i] = frozen[frozenInd];
        }
    }
    return word;
}