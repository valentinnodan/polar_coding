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
                           Message const &frozen) {
    size_t n = indices.size() + frozen.size();
    auto GN = getGN(n);
    std::cout << GN;
    auto GN_c = Matrix<Symbol>(0, n);
    auto GN_A = Matrix<Symbol>(0, n);
    for (size_t i = 0; i < n; ++i) {
        if (indices.count(i) != 0) {
            GN_A.addRow(GN.getRow(i));
        } else {
            GN_c.addRow(GN.getRow(i));
        }
    }
    std::cout << '\n';
    std::cout << GN_c;
    std::cout << '\n';
    std::cout << GN_A;
    auto x_A = GN_A.dot(u);
    auto x_c = GN_c.dot(frozen);
    auto codeWord = Message(x_A.size());
    for (size_t i = 0; i < x_A.size(); i++) {
        codeWord[i] = (x_A[i] + x_c[i]);
    }
    return codeWord;
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
    std::cout << res << '\n';
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

