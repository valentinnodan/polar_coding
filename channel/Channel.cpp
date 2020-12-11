//
// Created by valentinnodan on 11/13/20.
//

#include <random>
#include "Channel.h"


Message ChannelMatrix::Gauss(const Message &c, int n, int k) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    const double mySigma = sqrt(0.5 * pow(10, E / 10) * n / k);
    std::normal_distribution<> d{0, mySigma};
    auto res = Message();
    for (auto i : c) {
        if (i.symbol == 0) {
            res.emplace_back(-1 + d(gen));
        } else {
            res.emplace_back(1 + d(gen));
        }

    }

    return res;
}

// ???
double ChannelMatrix::getW(const Symbol &y, const Symbol &c, int n, int k) const {
    const double mySigma = pow(10, E / 10) * n / k;
    double m = sqrt(M_PI * mySigma);
    double d = pow((y.symbol - pow(-1, c.symbol)), 2);
    double deg = -1 * d / mySigma;
//    double e = exp(deg);
    return deg - log(m);

}
