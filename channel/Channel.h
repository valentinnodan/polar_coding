//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <vector>
#include <Symbol.h>
#include <cassert>
#include <cmath>


class ChannelMatrix {
private:
    double data[input_symbol_num * output_symbol_num]{};
    // E - отношение сигнал - шум
    const double E;
public:

    ChannelMatrix(double E) : data(), E(E) {
    }

    double getW(const Symbol &y, const Symbol &c, int n, int k) const;

    Message Gauss(const Message &c, int n, int k);
};
