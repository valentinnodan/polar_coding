//
// Created by valentinnodan on 11/5/20.
//

#include "PolarDecoder.h"

Message
PolarDecoder::decode(Message const &y, std::set<size_t> const &indices,
                     Message const &frozen) const {
    size_t currFrozenInd = 0;
    auto uI = Message(); // TODO: Why empty?
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

Symbol PolarDecoder::h(Message const &y, Message const &u) const {
    const double wNIZero = myChannel.getWNI(y, u, SymbolConsts::ZERO);
    const double wNIOne = myChannel.getWNI(y, u, SymbolConsts::ONE);
    if (wNIZero >=  wNIOne) {
        return SymbolConsts::ZERO;
    }
    return SymbolConsts::ONE;
}
