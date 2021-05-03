//
// Created by valentinnodan on 4/10/21.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "DecoderLP.h"

Message DecoderLP::decode(size_t n, size_t r, size_t needed, const std::vector<double> &llr,
                          const Matrix<size_t> &C,
                          const Matrix<size_t> &V,
                          size_t iter) const {
    auto l = Matrix<double>(n, r, 0);
    auto m = Matrix<double>(n, r, 0);
    auto x = std::vector<double>(n, 0);
    auto v = std::vector<double>(n, 0);
    auto sums = std::vector<double>(n, 0);
    for (size_t k = 0; k < iter; k++) {
        for (size_t i = 0; i < n; i++) {
//            double sum = 0;
//            for (size_t j = 1; j <= C[i][0]; j++) {
//                size_t jj = C[i][j];
//                sum += m[i][jj];
//            }
            x[i] = projectDot(penalize(sums[i] - llr[i]) / C[i][0]);
            sums[i] = 0;
        }

        for (size_t i = 0; i < r; i++) {
            for (size_t j = 1; j <= V[i][0]; j++) {
                size_t jj = V[i][j];
                v[j - 1] = x[jj] + l[jj][i];
            }
            auto z = projectPolytope(v, V[i][0]);
            for (size_t j = 1; j <= V[i][0]; j++) {
                size_t jj = V[i][j];
                l[jj][i] = v[j - 1] - z[j - 1];
                m[jj][i] = 2 * z[j - 1] - v[j - 1];
                sums[jj] += m[jj][i];
            }
        }
    }
    auto res = Message();
    for (size_t i = 0; i < needed; i++) {
        if (x[i + n - needed] > 0) {
            res.emplace_back(1);
        } else {
            res.emplace_back(0);
        }
    }
    return res;
}

double DecoderLP::penalize(double t) const {
    if (std::abs(t) < 1e-7) {
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
    auto f = std::vector<int>(s, 0);
    size_t sum = 0;
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

std::vector<double> DecoderLP::projectProbabilitySimplex(const std::vector<double> &v) const {
    size_t s = v.size();
    auto p = v;
    sort(p.begin(), p.end(), std::greater<>());
    auto u = std::vector<double>(s, 0);
    size_t maxInd = 0;
    for (size_t i = 1; i <= s; i++) {
        for (size_t j = 0; j < i; j++) {
            u[i - 1] += p[j];
        }
        u[i - 1] -= 1;
        u[i - 1] /= i;
        if (u[i - 1] < p[i - 1]) {
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
    auto w = std::vector<double>();
    for (size_t i = 0; i < s; i++) {
        double d = projectDot(vv[i]);
        sum += d;
        w.push_back(projectDot(v[i]));
    }
    if (sum >= 1 - (double) s / 2) {
        return w;
    } // todo split count w only one when needed
    return u;
}

int DecoderLP::powSign(int f) const {
    if (f == 1) {
        return -1;
    }
    return 1;
}
