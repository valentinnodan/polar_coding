//
// Created by valentinnodan on 2/18/21.
//

#include <iostream>
#include "PolarDecoderBP.h"


Message PolarDecoderBP::decode(const Message &y, const std::set<size_t> &indices, const Message &frozen,
                               const std::vector<size_t> &reversedIndexes, size_t N, size_t K) const {
    size_t n = y.size();
    size_t nn = log2(n);

    auto s = myChannel.getSigma(N, K);
    auto l = Matrix<double>(n, nn + 1, 0);
    auto r = Matrix<double>(n, nn + 1, 0);
    for (size_t i = 0; i < n; ++i) {
        l[i][nn] = 2 * y[i].symbol / s;
        if (indices.count(i) == 0) {
            r[i][0] = std::numeric_limits<double>::infinity();
        } else {
            r[i][0] = 0;
        }
    }

//    visualize(l, "INIT L:");
//
//    visualize(r, "INIT R:");


    auto res = Message();
    res.reserve(n);
    Symbol currU{0};
    for (size_t i = 0; i < ITER; i++) {
        updateLR(l, r);
    }

//    visualize(l, "L_FINAL:");
//    visualize(r, "R_FINAL:");

    for (size_t i = 0; i < n; i++) {
        if (l[i][nn] + r[i][nn] >= 0) {
            currU = SymbolConsts::ZERO;
        } else {
            currU = SymbolConsts::ONE;
        }
        res.push_back(currU);
    }
    return res;
}

void PolarDecoderBP::updateLR(Matrix<double> &l, Matrix<double> &r) const {
    Matrix<double> ll = Matrix(l);
    Matrix<double> rr = Matrix(r);
    for (size_t j = 0; j < l.width - 1; j++) {
        size_t block = (1ull << (l.width - j - 2));
        for (size_t u = 0; u < l.height / (block * 2); u++) {
            for (size_t v = 0; v < block; v++) {
                size_t i = u * block * 2 + v;
                l[i][j] = f(ll[i][j + 1], ll[i + block][j + 1] + rr[i + block][j]);
                l[i + block][j] = ll[i + block][j + 1] + f(ll[i][j + 1], rr[i][j]);
                r[i][j + 1] = f(rr[i][j], ll[i + block][j + 1] + rr[i + block][j]);
                r[i + block][j + 1] = rr[i + block][j] + f(ll[i][j + 1], rr[i][j]);
            }
        }
    }
}

double PolarDecoderBP::f(double x, double y) {
//    if (x == std::numeric_limits<double>::infinity()) {
//        return y;
//    }
//    if (y == std::numeric_limits<double>::infinity()) {
//        return x;
//    }
//    return log( (1 + exp(x + y)) / (exp(x) + exp(y)));
    if (x * y >= 0) {
        return 0.9375 * std::min(std::abs(x), std::abs(y));
    } else {
        return -0.9375 * std::min(std::abs(x), std::abs(y));
    }
}

void PolarDecoderBP::visualize(Matrix<double> const &l, std::string text) const {
    std::cout << text << std::endl;
    for (size_t i = 0; i < l.height; i++) {
        for (size_t j = 0; j < l.width; j++) {
            std::cout << l[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
