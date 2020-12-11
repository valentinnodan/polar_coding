//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <cmath>
#include <vector>
#include "Symbol.h"

std::vector<size_t> genReversedIndex(size_t i) {
    std::vector<size_t> res;
    res.resize(i);
    res[0] = 0;
    for (size_t i1 = 1; i1 < i; i1 <<= 1) {
        for (size_t j = 0; j < i1; ++j) {
            res[j] <<= 1;
            res[j + i1] = res[j] + 1;
        }
    }
    return res;
}

std::pair<Message, Message> getOddsEvens(Message u) {
    auto resEvens = std::vector<Symbol>();
    auto resOddEvens = std::vector<Symbol>();
    for (size_t i = 1; i < u.size(); i += 2){
        resEvens.push_back(u[i]);
        resOddEvens.push_back(Symbol((int)(u[i].symbol + u[i - 1].symbol) % 2));
    }
    return std::pair<Message, Message>(resOddEvens, resEvens);
}