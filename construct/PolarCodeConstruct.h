//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <set>
#include <algorithm>
#include <iostream>
#include "../channel/Channel.h"

class PolarCodeConstruct {
public:
    double eps;

    double static getAWGN_eps(double eps) {
        return exp(-pow(10, eps / 10));
    }

    explicit PolarCodeConstruct(double eps) : eps(getAWGN_eps(eps)) {
    };

    std::set<size_t> constructBase(size_t n, size_t k, std::vector<double> &v) {
        std::vector<size_t> idx(n);
        for (size_t i = 0; i < n; i++) {
            idx[i] = i;
        }
        std::stable_sort(idx.begin(), idx.end(),
                         [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });
        auto res = std::set<size_t>();
        size_t count = 0;
        for (size_t i : idx) {
            if (++count > k) {
                break;
            }
            res.emplace(i);
        }
        return res;
    }

    std::set<size_t> construct(size_t n, size_t k) {
        auto v = getZ(n);

        return constructBase(n, k, v);
    }

    std::set<size_t> construct_AWGN(size_t n, size_t k) {
        auto v = getL(n);

        return constructBase(n, k, v);
    }

private:
    std::vector<double> getZ(size_t n);

    double getZ(size_t n, size_t i);

    std::vector<double> getL(size_t n);

    double getL(size_t n, size_t i);

    static double f(double l);
};

