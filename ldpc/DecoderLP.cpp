//
// Created by valentinnodan on 4/10/21.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "DecoderLP.h"

Message DecoderLP::decode(size_t needed, const std::vector<double> &llr,
                          const Matrix<size_t> &Nv,
                          const Matrix<size_t> &Nc,
                          size_t iter) {
    assert (n == Nv.height);
    assert (r == Nc.height);
    auto &l = data.lMatrix;
    auto &m = data.mMatrix;
    auto &prevZ = data.prevZ;
    l.zero();
    m.zero();
    prevZ.zero();
    auto &sums = data.sums;
    std::fill(sums.begin(), sums.end(), 0);

    auto &x = data.decode_x; //std::vector<double>(n, 0);
    auto &v = data.decode_v; //std::vector<double>(n, 0); // max deg

    for (size_t i = 0; i < n; i++) {
        sums[i] = penalize(-llr[i])/mu;
    }

    for (size_t k = 0; k < iter; k++) {
//        std::swap(prevL, l);
        for (size_t i = 0; i < r; i++) {
            for (size_t j = 1; j <= Nc[i][0]; j++) {
                size_t jj = Nc[i][j];
                x[jj] = projectDot(sums[jj] / (Nv[jj][0]  - 2 * alpha / mu));
                v[j - 1] = ro * x[jj] + l[i][j - 1] + (1 - ro)*prevZ[i][j - 1];
            }
            auto &z = projectPolytope(v, Nc[i][0]);
            for (size_t j = 1; j <= Nc[i][0]; j++) {
                size_t jj = Nc[i][j];
                l[i][j - 1] = v[j - 1] - z[j - 1];
                sums[jj] -= m[i][jj];
                m[i][jj] = 2 * z[j - 1] - v[j - 1];
                sums[jj] += m[i][jj];
                prevZ[i][j - 1] = z[j - 1];
            }
        }
    }
    auto res = Message();
    res.reserve(needed);
    for (size_t i = 0; i < needed; i++) {
        size_t i1 = i + n - needed;
        double tX = projectDot(sums[i1] / Nv[i1][0]);
        if (tX > 0) {
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

__attribute__((always_inline))
std::vector<double> &DecoderLP::projectPolytope(const std::vector<double> &v, size_t s) {
    auto &f = data.xl1;// std::vector<int>(s, 0);
    std::fill(f.begin(), f.begin() + s, 0);
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
    auto &vv = data.xl2; //std::vector<double>(s, 0);
    for (size_t i = 0; i < s; i++) {
        vv[i] = v[i] * powSign(f[i]);
    }
    return membershipTest(vv, f, v, s);
}

std::vector<double> &DecoderLP::projectProbabilitySimplex(const std::vector<double> &v, size_t s) {
    auto &p = data.xl3;
    std::copy(v.begin(), v.begin() + s, p.begin());
    std::sort(p.begin(), p.begin() + s, std::greater<>());
    double u_i = 0;
    double prev = 0;
    for (size_t i = 1; i <= s; i++) {
        prev += p[i - 1];
        double u = (prev - 1);
        if (u < p[i - 1] * i) {
            u_i = u / i;
        }
    }
    auto &w = data.xl4;// std::vector<double>(s, 0);
    for (size_t i = 0; i < s; i++) {
        w[i] = projectDot(v[i] - u_i - 0.5);
    }
    return w;
}

std::vector<double> &DecoderLP::membershipTest(const std::vector<double> &vv, std::vector<int> &f,
                                              std::vector<double> const &v,  size_t s) {
    double sum = 0;
    for (size_t i = 0; i < s; i++) {
        sum += projectDot(vv[i]);
    }
    if (sum >= 1 - ((double) s) / 2) {
        auto &u = data.xl4;
        for (size_t i = 0; i < s; i++) {
            u[i] = projectDot(v[i]);
        }
        return u;
    } else {
        auto &u = projectProbabilitySimplex(vv, s);
        for (size_t i = 0; i < s; i++) {
            u[i] = u[i] * powSign(f[i]);
        }
        return u;
    }
}

int DecoderLP::powSign(int f) {
    if (f == 1) {
        return -1;
    }
    return 1;
}
