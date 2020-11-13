//
// Created by valentinnodan on 11/5/20.
//

#include <iostream>
#include "PolarCoder.h"

const Matrix<uint64_t> PolarCoder::F = Matrix<uint64_t>(2, 2, std::vector<uint64_t>{1, 0, 1, 1});
const Matrix<uint64_t> PolarCoder::I2 = Matrix<uint64_t>(2);

Matrix<uint64_t> PolarCoder::getFN(int n) {
    Matrix<uint64_t> myRes = F;
    for (int i = 0; i < n - 1; i++) {
        myRes = F * myRes;
    }
    return myRes;
}

std::vector<uint64_t> PolarCoder::encode(std::vector<uint64_t> const &u, std::set<size_t> const &indices,
                                         std::vector<uint64_t> const &frozen) {
    size_t n = indices.size() + frozen.size();
    auto GN = getGN(log2(n));
    std::cout << GN;
    auto GN_c = Matrix<uint64_t>(0, n);
    auto GN_A = Matrix<uint64_t>(0, n);
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
    auto codeWord = std::vector<uint64_t>(x_A.size());
    for (size_t i = 0; i < x_A.size(); i++) {
        codeWord[i] = (x_A[i] + x_c[i]) % 2;
    }
    return codeWord;
}

Matrix<uint64_t> PolarCoder::RN(Matrix<uint64_t> m) {
    auto res = Matrix<uint64_t>(m.height, m.width);
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

Matrix<uint64_t> PolarCoder::getBN(int n) {
    if (n == 2) {
        return I2;
    }
    auto B = getBN(n / 2);
    auto multRes = I2 * B;
    auto r = RN(multRes);
    return r;
}

Matrix<uint64_t> PolarCoder::getGN(int n) {
    size_t nn = log2(n);
    auto fN = getFN(nn);
    auto BN = getBN(n);
    return Matrix<uint64_t>::mult(BN, fN);
}

