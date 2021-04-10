//
// Created by valentinnodan on 4/10/21.
//

#pragma once

#include <Symbol.h>
#include <set>
#include "../channel/Channel.h"

class PolarDecoder {
public:
    explicit PolarDecoder(Channel const & channel): myChannel(channel){};

    virtual Message decode(const Message &y, const std::set<size_t> &indices, size_t N, size_t K ) const = 0;

protected:
    Channel myChannel;

};
