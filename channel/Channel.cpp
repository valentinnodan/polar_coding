//
// Created by valentinnodan on 11/13/20.
//

#include "Channel.h"


Message Channel::Gauss(const Message &c, int n, int k) const {
    const double mySigma = sqrt(getSigma(n, k));
    std::normal_distribution<> d{0, mySigma};
    auto res = Message();
    for (auto i : c) {
        auto t = d(gen);
        if (i.symbol == 0) {
            auto val =  1.0 + t;
            res.emplace_back(val);
        } else {
            res.emplace_back(t - 1);
        }
    }
    return res;
}


double Channel::getSigma(int n, int k) const {
    return 0.5 * pow(10, -E / 10) * ((double)n / k);
}
