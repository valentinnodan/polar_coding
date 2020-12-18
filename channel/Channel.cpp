//
// Created by valentinnodan on 11/13/20.
//

#include <random>
#include <iostream>
#include "Channel.h"


Message Channel::Gauss(const Message &c, int n, int k) const {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    const double mySigma = sqrt(getSigma(n, k));
    std::normal_distribution<> d{0, mySigma};
    auto res = Message();
    for (auto i : c) {
        auto t = d(gen);
        if (i.symbol == 0) {
            auto val =  t - 1;
            res.emplace_back(val);
        } else {
            res.emplace_back(1.0 + t);
        }

    }

    return res;
}


double Channel::getSigma(int n, int k) const {
    return 0.5 * pow(10, -E / 10) * ((double)n / k);
}
