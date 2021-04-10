//
// Created by valentinnodan on 11/5/20.
//

#include "PolarCodeConstruct.h"

std::vector<double> PolarCodeConstruct::getZ(size_t n) {
    auto res = std::vector<double>(n);
    for (size_t i = 1; i <= n; ++i) {
        const double curr = getZ(n, i);
        assert(curr <= 1.0 && curr >= 0.0);
        res[i - 1] = curr;
    }
    return res;
}

double PolarCodeConstruct::getZ(size_t n, size_t i) {
    if (n == 1 && i == 1) {
        return eps;
    }
    if (i % 2 == 1) {
        double zValue = getZ(n / 2, ((i + 1) / 2));
        return 2 * zValue - zValue * zValue;
    }
    const double z = getZ(n / 2, i / 2);
    return z * z;
}

std::vector<double> PolarCodeConstruct::getL(size_t n) {
    auto res = std::vector<double>(n);
    for (size_t i = 1; i <= n; ++i) {
        const double curr = getL(n, i);
        assert(curr <= 1.0 && curr >= 0.0);
        res[i - 1] = curr;
    }
    return res;
}

double PolarCodeConstruct::getL(size_t n, size_t i) {
    if (n == 1 && i == 1) {
        return 2 / eps;
    }
    if (i % 2 == 1) {
        double val = getL(n - 1, (i + 1) / 2);
        return 2 * val;
    }

    double val = getL(n - 1, i / 2);
    return f(val);
}

double PolarCodeConstruct::f(double l) {
    if (l > 12) {
        return 0.9861 * l - 2.3152;
    } else if (l > 3.5) {
        return l * (9.005 * 0.001 * l + 0.7694) - 0.9507;
    } else if (l > 1) {
        return l * (0.062883 * l + 0.3678) - 0.1627;
    } else {
        return l * (0.2202 * l + 0.06448);
    }
}
