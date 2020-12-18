//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <vector>
#include <Symbol.h>
#include <cassert>
#include <cmath>


class Channel {
private:
    // E - отношение сигнал - шум

public:
    const double E;
    explicit Channel(double E) : E(E) {
    }

    Message Gauss(const Message &c, int n, int k) const;

    [[nodiscard]] double getSigma(int n, int k) const;
};
