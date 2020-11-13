//
// Created by valentinnodan on 11/5/20.
//

#include "PolarDecoder.h"

Message
PolarDecoder::decode(Message const &y, std::set<size_t> const &indices,
                     Message const &frozen) {
    int currFrozenInd = 0;
    auto uI = Message();
    auto res = Message(y.size());
    for (size_t i = 0; i < y.size(); i++) {
        Symbol currU{0};
        if (indices.count(i) == 0) {
            currU = frozen[currFrozenInd];
            res[i] = currU;
            currFrozenInd++;
        } else {
            currU = h(y, uI);
            res[i] = currU;
        }
        uI.push_back(currU);
    }
    return res;
}

Symbol PolarDecoder::h(Message const &y, Message const &u) {
    double wNIZero = myChannel.getWNI(y, u, Symbol::ZERO);
    double wNIOne = myChannel.getWNI(y, u, Symbol::ONE);
    if (wNIZero / wNIOne >= 1) {
        return Symbol::ZERO;
    }
    return Symbol::ONE;
}
