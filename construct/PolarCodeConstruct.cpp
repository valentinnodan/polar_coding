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
        return c.getEps();
    }
    if (i % 2 == 1) {
        double zValue = getZ(n / 2, ((i + 1) / 2));
        return 2 * zValue - zValue * zValue;
    }
    const double z = getZ(n / 2, i / 2);
    return z * z;
}
