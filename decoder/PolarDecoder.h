//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <vector>
#include <cstdint>
#include <set>

#include "Symbol.h"
#include "../channel/Channel.h"

class PolarDecoder {
public:
    explicit PolarDecoder(ChannelMatrix const & m): myChannel(m){}
    Message
    decode(Message const &y, std::set<size_t> const &indices, Message const &frozen);

private:
    Symbol h(Message const &y, Message const &u);
    ChannelMatrix myChannel;
};

