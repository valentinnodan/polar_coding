//
// Created by valentinnodan on 11/13/20.
//

#include <random>
#include "Channel.h"

double ChannelMatrix::getWN(const Message &y, const Message &c) {
    double res = 1;
    for (size_t i = 0; i < y.size(); ++i) {
        res *= (*this)[y[i]][c[i]];
    }
    return res;
}

void increase(Message &u) {
    for (size_t i = u.size(); i--;) {
        if (u[i].symbol == 0) {
            u[i].symbol = 1;
            break;
        } else {
            u[i].symbol = 0;
        }
    }
}

double ChannelMatrix::getWNI(const Message &y, const Message &u, Symbol s) {
    size_t k = y.size() - u.size() - 1;
    double res = 0;
    auto curr = Message(u.size() + k, SymbolConsts::ZERO);
    std::copy(u.begin(), u.end(), curr.begin());
    curr[u.size()] = s;
    for (uint64_t i = 0; i < 1ull << k; i++) {
        res += getWN(y, curr);
        increase(curr);
    }
    res /= static_cast<double>(1ull << (y.size() - 1));
    return res;
}

Message ChannelMatrix::BEC(Message c) {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    for (int n = 0; n < 10; ++n) {
        double curr = dis(gen);
//        if (curr < )
    }
    return Message();
}

double ChannelMatrix::getEps() {
    return (*this)[SymbolConsts::EPS][SymbolConsts::ZERO];
}
