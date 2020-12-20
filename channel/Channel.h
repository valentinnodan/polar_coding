//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <vector>
#include <Symbol.h>
#include <cassert>
#include <cmath>
#include <random>


class Channel {
private:
    // E - отношение сигнал - шум

public:
    const double E;
    std::random_device rd;
    mutable std::mt19937 gen;

    explicit Channel(double E) : E(E), gen(rd()) {
    }

    Channel(Channel const &other) : E(other.E), gen(rd()) {

    }

    Message Gauss(const Message &c, int n, int k) const;

    [[nodiscard]] double getSigma(int n, int k) const;
};
