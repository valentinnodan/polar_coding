//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <chrono>
#include "Symbol.h"
#include "Transform.h"
#include "../construct/PolarCodeConstruct.h"
#include "../decoder/PolarDecoderBP.h"
#include "../coder/PolarCoder.h"

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
        std::cout << cW[i] << " ";
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

inline void runSimulation(double becProbability,
                          size_t N,
                          size_t K,
                          size_t wordsAmount) {
    auto frozen = Message(N - K);

    auto cPC = PolarCodeConstruct(becProbability);
    auto A = cPC.construct(N, K);

    auto reversedIndexes = genReversedIndex(N);
    auto revPlace = std::vector<size_t>(N);
    for (size_t i = 0; i < N; i++) {
        revPlace[reversedIndexes[i]] = i;
    }

    std::chrono::microseconds totalEncoder;
    std::chrono::microseconds totalDecoder;

    auto H = transform(N, A);
//    std::cout << H;
    for (size_t i = 1;  i < 11; i++) {
        auto gaussianChannel = Channel(i);
        const auto decoder = PolarDecoderBP(gaussianChannel);
        int e = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            auto begin = std::chrono::high_resolution_clock::now();
            const auto cW = PolarCoder::encode(myMsg, A, frozen, reversedIndexes);
            auto end = std::chrono::high_resolution_clock::now();
            totalEncoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

            auto word = PolarCoder::getWord(A, frozen, myMsg);
//            printWord(word);
            auto gaussWord = gaussianChannel.Gauss(cW, N, K);
//            printWord(gaussWord);
//            printWord(cW);
            begin = std::chrono::high_resolution_clock::now();
            auto decodedWord = decoder.decode(gaussWord, A, frozen, revPlace, N, K, H);
//            printWord(decodedWord);
//            std::cout << std::endl;
            end = std::chrono::high_resolution_clock::now();
            totalDecoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            if (compareWords(decodedWord, cW) > 0) {
//                std::cout << compareWords(decodedWord, cW) << std::endl;
                e += 1;
            }
//            printWord(decodedWord);
        }
        std::cout << "Decoded word error:  " << (double) e / (wordsAmount) << " Eb_N0: " << (double) i
                  << std::endl;

    }

//    std::cout << "Total encoder time: " << totalEncoder.count() << " microseconds" << std::endl;
//    std::cout << "Total decoder time: " << totalDecoder.count() << " microseconds" << std::endl;

}