//
// Created by valentinnodan on 11/13/20.
//

#include <random>
#include "Channel.h"
#include "../coder/PolarCoder.h"

double ChannelMatrix::getWN(Message const &y, Message const &u) const {
    auto c = PolarCoder::getGN(y.size()).dot(u);
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

double ChannelMatrix::getWNI(const Message &y, const Message &u, Symbol s) const {
    size_t k = y.size() - u.size() - 1;
    double res = 0;
    auto curr = Message(u.size() + k + 1, SymbolConsts::ZERO);
    std::copy(u.begin(), u.end(), curr.begin());
    curr[u.size()] = s;
    assert(curr.size() == y.size());
    for (uint64_t i = 0; i < 1ull << k; i++) {
        res += getWN(y, curr);
        increase(curr);
    }
    res /= static_cast<double>(1ull << (y.size() - 1));
    return res;
}

Message ChannelMatrix::BEC(Message const &c) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    Message output;
    for (auto symb : c) {
        double curr = dis(gen);
        double sum = 0.0;
        for (size_t i = 0; i < output_symbol_num; ++i) {
            sum += (*this)[Symbol(i)][symb];
            if (sum >= curr) {
                output.push_back(Symbol(i));
                break;
            }
        }
    }
    return output;
}

double ChannelMatrix::getEps() const {
    return (*this)[SymbolConsts::EPS][SymbolConsts::ZERO];
}
