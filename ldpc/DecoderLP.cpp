//
// Created by valentinnodan on 4/10/21.
//

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include "DecoderLP.h"

Message DecoderLP::decode(size_t n, size_t r, size_t needed, const std::vector<double> &llr,
                          const Matrix<size_t> &c,
                          const Matrix<size_t> &v,
                          size_t iter) const {
    auto l = Matrix<double>(n, r, 0);
    auto m = Matrix<double>(n, r, 0);
    auto x = std::vector<double>(r, 0);
    auto vv = std::vector<double>(r, 0);
    for (size_t k = 0; k < iter; k++) {
        for (size_t i = 0; i < r; i++) {
            double sum = 0;
            for (size_t j = 1; j <= v[i][0]; j++) {
                size_t jj = v[i][j];
                sum += m[jj][i];
            }
            sum = penalize(sum - llr[i]);
            x[i] = projectDot(sum / v[i][0]);
        }
        for (size_t i = 0; i < n; i++) {
            size_t lInd = 0;
            for (size_t j = 1; j <= c[i][0]; j++) {
                size_t jj = c[i][j];
                vv[lInd++] = x[jj] + l[i][jj];
            }
            auto w = projectPolytope(vv, c[i][0]);
            for (size_t j = 0; j < c[i][0]; j++) {
                size_t jj = c[i][j + 1];
                l[i][jj] = vv[j] - w[j];
                m[i][jj] = 2 * w[j] - vv[j];
            }
        }
    }
    auto res = Message(r);
    for (size_t i = 0; i < r; i++) {
        if (x[i] > 0) {
            res[i] = SymbolConsts::ONE;
        } else {
            res[i] = SymbolConsts::ZERO;
        }
    }
    return res;
}

double DecoderLP::penalize(double t) const {
    if (std::abs(t) < 1e-10) {
        return t;
    }
    if (t > 0) {
        return t + alpha;
    }
    return t - alpha;
}

double DecoderLP::projectDot(double dot) const {
    if (dot > 0.5) {
        return 0.5;
    }
    if (dot < -0.5) {
        return -0.5;
    }
    return dot;
}

std::vector<double> DecoderLP::projectPolytope(const std::vector<double> &v, size_t s) const {
    auto f = std::vector<double>(s, 0.);
    size_t sum = 0;
    double minV = std::numeric_limits<double>::infinity();
    size_t minInd = 0;
    for (size_t i = 0; i < s; i++) {
        if (v[i] >= 0) {
            f[i] = 1;
            sum += 1;
        }
        if (std::abs(v[i]) < minV) {
            minV = std::abs(v[i]);
            minInd = i;
        }
    }
    if (sum % 2 == 0) {
        f[minInd] = 1 - f[minInd];
    }
    auto vv = std::vector<double>(s, 0);
    for (size_t i = 0; i < s; i++) {
        vv[i] = v[i] * pow(-1, f[i]);
    }
    auto u = projectProbabilitySimplex(vv);
    for (size_t i = 0; i < s; i++) {
        u[i] = u[i] * pow(-1, f[i]);
    }
    return membershipTest(vv, u, v);
}

std::vector<double> DecoderLP::projectProbabilitySimplex(const std::vector<double> &v) const {
    size_t s = v.size();
    auto p = v;
    sort(p.begin(), p.end(), std::greater<>());
    auto u = std::vector<double>(s, 0);
    size_t maxInd = 0;
    for (size_t i = 1; i <= s; i++) {
        for (size_t j = 0; j < i; j++) {
            u[i-1] += p[j];
        }
        u[i-1] -= 1;
        u[i-1] /= i;
        if (u[i-1] < p[i-1]) {
            maxInd = i - 1;
        }
    }
    auto w = std::vector<double>(s, 0);
    for (size_t i = 0; i < s; i++) {
        w[i] = std::max(v[i] - u[maxInd] - 0.5, -0.5);
    }
    return w;
}

std::vector<double> DecoderLP::membershipTest(const std::vector<double> &vv, const std::vector<double> &u,
                                              std::vector<double> const &v) const {
    size_t s = vv.size();
    double sum = 0;
    auto w = std::vector<double>(s);
    for (size_t i = 0; i < s; i++) {
        double d = projectDot(vv[i]);
        sum += d;
        w[i] = projectDot(v[i]);
    }
    if (sum >= 1 - (double) s / 2) {
        return w;
    } // todo split count w only one when needed
    return u;
}
