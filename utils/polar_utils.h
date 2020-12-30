//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "Symbol.h"

inline std::vector<size_t> genReversedIndex(size_t i) {
    std::vector<size_t> res;
    res.resize(i);
    res[0] = 0;
    for (size_t i1 = 1; i1 < i; i1 <<= 1u) {
        for (size_t j = 0; j < i1; ++j) {
            res[j] <<= 1u;
            res[j + i1] = res[j] + 1;
        }
    }
    return res;
}

inline std::pair<Message, Message> getOddsEvens(Message const &u, size_t uLength) {
    auto resEvens = std::vector<Symbol>(uLength / 2);
    auto resOddEvens = std::vector<Symbol>(uLength / 2);
    for (size_t i = 1; i < uLength; i += 2) {
        auto ind = (i - 1) / 2;
        resEvens[ind] = u[i];
        resOddEvens[ind].symbol = (int) (u[i].symbol + u[i - 1].symbol) % 2;
    }
    return std::pair<Message, Message>(resOddEvens, resEvens);
}

inline void printWord(Message const &cW) {
    std::cout << "Word:    ";
    for (size_t i = 0; i < cW.size(); ++i) {
        std::cout << cW[i];
    }
    std::cout << "\n";
}

inline Message getRandomWord(size_t length) {
    auto res = Message(length);
    for (size_t i = 0; i < length; ++i) {
        res[i].symbol = rand() % 2;
    }
    return res;
}

inline int compareWords(Message const &a, Message const &b) {
    assert(a.size() == b.size());
    int res = 0;
    for (size_t j = 0; j < a.size(); ++j) {
        double diff = a[j].symbol - b[j].symbol;
        if (std::abs(diff) > Symbol::EPS) {
            res++;
        }
    }
    return res;
}