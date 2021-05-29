//
// Created by valentinnodan on 4/10/21.
//

#pragma once

#include <Symbol.h>
#include <set>
#include "../channel/Channel.h"

class PolarDecoder {
public:
    explicit PolarDecoder(Channel const & channel, size_t n , size_t k): myChannel(channel), n(n), k(k) {};

    virtual Message decode(const Message &y, const std::set<size_t> &indices) = 0;

protected:
    Channel myChannel;
    size_t n;
    size_t k;

};
