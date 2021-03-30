//
// Created by valentinnodan on 3/21/21.
//

#include <cmath>
#include <iostream>
#include "DecoderBP.h"

Message DecoderBP::decode(size_t n, size_t r, size_t needed, const std::vector<double> &llr,
                          const Matrix<size_t> &c,
                          const Matrix<size_t> &v,
                          const size_t iter) const {
    auto l = Matrix<double>(r, n, 0);
    auto z = Matrix<double>(r, n, 0);
    for (size_t k = 0; k < iter; k++) {
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < r; j++) {
                l[j][i] = llr[i];
                for (size_t h = 1; h <= c[i][0]; h++) {
                    size_t hh = c[i][h];
                    if (hh != j) {
                        l[j][i] += z[hh][i];
                    }
                }
            }
        }
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 1; j <= c[i][0]; j++) {
                int mul = 1;
                double sum = 0;
                size_t jj = c[i][j];
                for (size_t h = 1; h <= v[jj][0]; h++) {
                    size_t hh = v[jj][h];
                    if (hh != i) {
                        if (l[jj][hh] < 0) {
                            mul *= -1;
                        }
                        sum += f(std::abs(l[jj][hh]));
                    }
                }
                z[jj][i] = std::max(std::min(mul * f(std::abs(sum)), 19.07), -19.07);
            }
        }
    }


    auto res = Message();
    for (size_t i = n - needed; i < n; i++) {
        double sum = 0;
        for (size_t h = 1; h <= c[i][0]; h++) {
            size_t hh = c[i][h];
            sum += z[hh][i];
        }
        if (llr[i] + sum < 0) {
            res.emplace_back(1);
        } else {
            res.emplace_back(0);
        }
    }
    return res;
}

double DecoderBP::f(double x) const {
    if (std::abs(x) > 20) {
        return 0;
    }
    if (std::abs(x) < 1e-12) {
        return std::numeric_limits<double>::infinity();
    }
    double eX = exp(std::abs(x));
    return log((eX + 1) / (eX - 1));
}

std::pair<Matrix<size_t>,
        Matrix<size_t>> DecoderBP::prepare(const Matrix<int> &h) {
    auto c = std::vector<std::unordered_set<size_t>>(h.height);
    auto v = std::vector<std::unordered_set<size_t>>(h.width);
    for (size_t i = 0; i < h.height; i++) {
        for (size_t j = 0; j < h.width; j++) {
            if (h[i][j] == 1) {
                c[i].emplace(j);
                v[j].emplace(i);
            }
        }
    }
    size_t j = 0;
    Matrix<size_t> cc = Matrix<size_t>(h.height, h.width, 0);
    Matrix<size_t> vv = Matrix<size_t>(h.width, h.height, 0);
    for (size_t i = 0; i < h.height; i++) {
        cc[i][0] = c[i].size();
        j = 1;
        for (size_t h: c[i]) {
            cc[i][j] = h;
            j++;
        }
    }

    for (size_t i = 0; i < h.width; i++) {
        vv[i][0] = v[i].size();
        j = 1;
        for (size_t h: v[i]) {
            vv[i][j] = h;
            j++;
        }
    }
    return std::make_pair(cc, vv);
}
