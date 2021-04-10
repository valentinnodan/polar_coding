//
// Created by valentinnodan on 3/21/21.
//

#include <cmath>
#include "DecoderBP.h"

Message DecoderBP::decode(size_t n, size_t r, size_t needed, const std::vector<double> &llr,
                          const Matrix<size_t> &c,
                          const Matrix<size_t> &v,
                          const size_t iter) const {
    auto l = Matrix<double>(n, r, 0);
    auto z = Matrix<double>(n, r, 0);
    for (size_t k = 0; k < iter; k++) {
        for (size_t i = 0; i < n; i++) {
            double sumL = 0;
            for (size_t h = 1; h <= c[i][0]; h++) {
                size_t hh = c[i][h];
                sumL += z[i][hh];
            }
            for (size_t j = 1; j <= c[i][0]; j++) {
                size_t jj = c[i][j];
                l[i][jj] = llr[i] + sumL - z[i][jj];
            }
        }
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 1; j <= c[i][0]; j++) {
                size_t jj = c[i][j];

                int mul = 1;
                double sum = 0;
                for (size_t h = 1; h <= v[jj][0]; h++) {
                    size_t hh = v[jj][h];
                    if (hh != i) {
                        if (l[hh][jj] < 0) {
                            mul *= -1;
                        }
                        sum += f(std::abs(l[hh][jj]));
                        if (sum > 19.07) {
                            break;
                        }
                    }
                }
                z[i][jj] = std::max(std::min(mul * f(sum), 19.07), -19.07);
                assert(!std::isnan(z[i][jj]));
            }
        }
    }


    auto res = Message();
    for (size_t i = n - needed; i < n; i++) {
        double sum = 0;
        for (size_t h = 0; h < r; h++) {
            sum += z[i][h];
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
    if (std::abs(x) > 19.07) {
        return 0;
    }
    double eX = std::exp(std::abs(x));
    return std::abs(std::log((eX + 1) / (eX - 1)));
}

std::pair<Matrix<size_t>,
        Matrix<size_t>> DecoderBP::prepare(const Matrix<int> &h) {
    auto c = std::vector<std::unordered_set<size_t>>(h.width);
    auto v = std::vector<std::unordered_set<size_t>>(h.height);
    for (size_t i = 0; i < h.height; i++) {
        for (size_t j = 0; j < h.width; j++) {
            if (h[i][j] == 1) {
                c[j].emplace(i);
                v[i].emplace(j);
            }
        }
    }
    size_t j;
    Matrix<size_t> cc = Matrix<size_t>(h.width, 10, 0);
    Matrix<size_t> vv = Matrix<size_t>(h.height, 10, 0);
    for (size_t i = 0; i < h.width; i++) {
        cc[i][0] = c[i].size();
        j = 1;
        for (size_t h1: c[i]) {
            cc[i][j] = h1;
            j++;
        }
    }

    for (size_t i = 0; i < h.height; i++) {
        vv[i][0] = v[i].size();
        j = 1;
        for (size_t h1: v[i]) {
            vv[i][j] = h1;
            j++;
        }
    }
    return std::make_pair(cc, vv);
}
