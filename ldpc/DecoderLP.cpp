//
// Created by valentinnodan on 4/10/21.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "DecoderLP.h"

Message DecoderLP::decode(size_t n, size_t r, size_t needed, const std::vector<double> &llr,
                          const Matrix<size_t> &Nv,
                          const Matrix<size_t> &Nc,
                          size_t iter) const {
    auto l = Matrix<double>(r, n, 0);
    auto x = std::vector<double>(n, 0);
    auto v = std::vector<double>(n, 0);
    auto sums = std::vector<double>(n, 0);
    for (size_t k = 0; k < iter; k++) {
        for (size_t i = 0; i < n; i++) {
            x[i] = projectDot(penalize(sums[i] - llr[i]) / Nv[i][0]);
            sums[i] = 0;
        }

        for (size_t i = 0; i < r; i++) {
            for (size_t j = 1; j <= Nc[i][0]; j++) {
                size_t jj = Nc[i][j];
                v[j - 1] = x[jj] + l[i][j - 1];
            }
            auto z = projectPolytope(v, Nc[i][0]);
            for (size_t j = 1; j <= Nc[i][0]; j++) {
                size_t jj = Nc[i][j];
                l[i][j - 1] = v[j - 1] - z[j - 1];
                sums[jj] += 2 * z[j - 1] - v[j - 1];
            }
        }
    }
    auto res = Message();
    res.reserve(needed);
    for (size_t i = 0; i < needed; i++) {
        if (x[i + n - needed] == 0) {
            res.emplace_back(rand() % 2);
        }
        if (x[i + n - needed] > 0) {
            res.emplace_back(1);
        } else {
            res.emplace_back(0);
        }
    }
    return res;
}

double DecoderLP::penalize(double t) const {
    if (std::abs(t) == 0) {
        return t;
    }
    if (t > 0) {
        return t + alpha;
    }
    return t - alpha;
}

double DecoderLP::projectDot(double dot) {
    if (dot > 0.5) {
        return 0.5;
    }
    if (dot < -0.5) {
        return -0.5;
    }
    return dot;
}

std::vector<double> DecoderLP::projectPolytope(const std::vector<double> &v, size_t s) {
    auto f = std::vector<int>(s, 0);
    int sum = 0;
    double minV = std::numeric_limits<double>::infinity();
    size_t minInd = -1;
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
        vv[i] = v[i] * powSign(f[i]);
    }
    auto u = projectProbabilitySimplex(vv);
    for (size_t i = 0; i < s; i++) {
        u[i] = u[i] * powSign(f[i]);
    }
    return membershipTest(vv, u, v);
}

std::vector<double> DecoderLP::projectProbabilitySimplex(const std::vector<double> &v) {
    size_t s = v.size();
    auto p = v;
    std::sort(p.begin(), p.end(), std::greater<>());
    double u_i = 0;
    double prev = 0;
    for (size_t i = 1; i <= s; i++) {
        prev += p[i - 1];
        double u = (prev - 1) / i;
        if (u < p[i - 1]) {
            u_i = u;
        }
    }
    auto w = std::vector<double>(s, 0);
    for (size_t i = 0; i < s; i++) {
        w[i] = std::max(v[i] - u_i - 0.5, -0.5);
    }
    return w;
}

std::vector<double> DecoderLP::membershipTest(const std::vector<double> &vv, const std::vector<double> &u,
                                              std::vector<double> const &v) {
    size_t s = vv.size();
    double sum = 0;
    for (size_t i = 0; i < s; i++) {
        sum += projectDot(vv[i]);
    }
    if (sum >= 1 - ((double) s) / 2) {
        auto w = std::vector<double>(s);
        for (size_t i = 0; i < s; i++) {
            w[i] = projectDot(v[i]);
        }
        return w;
    }
    return u;
}

int DecoderLP::powSign(int f) {
    if (f == 1) {
        return -1;
    }
    return 1;
}
