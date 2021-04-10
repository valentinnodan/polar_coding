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
#include "../ldpc/DecoderBP.h"

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
                          size_t wordsAmount,
                          bool isSystematic,
                          bool isBER) {
    auto reversedIndexes = genReversedIndex(N);
    auto revPlace = std::vector<size_t>(N);
    for (size_t i = 0; i < N; i++) {
        revPlace[reversedIndexes[i]] = i;
    }
    auto frozen = Message(N - K);

    std::chrono::microseconds totalEncoder;
    std::chrono::microseconds totalDecoder;

    auto H = transform(N, std::set<size_t>());
    auto G = PolarCoder::getGN(N);
    auto pair = DecoderBP::prepare(H);

    for (size_t i = 2; i < 20; i++) {
        double ii = (double) i / 2;
        auto gaussianChannel = Channel(ii);
        auto cPC = PolarCodeConstruct(ii);
        auto A = cPC.construct(N, K);
        auto AA = std::set<size_t>();
        for (size_t j : A) {
            AA.emplace(reversedIndexes[j]);
        }
        A = AA;

        const auto decoder = PolarDecoderBP(gaussianChannel, pair);
//        const auto decoderSC = PolarDecoderSC(gaussianChannel);
        int e = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            auto begin = std::chrono::high_resolution_clock::now();

            auto cW = PolarCoder::encode(myMsg, A, frozen, reversedIndexes);
            if (isSystematic) {
                auto cc = Message();
                for (size_t t = 0; t < N; t++) {
                    if (A.count(t) == 1) {
                        cc.push_back(cW[t]);
                    }
                }
                cW = PolarCoder::encode(cc, A, frozen, reversedIndexes);
            }

            auto end = std::chrono::high_resolution_clock::now();
            totalEncoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

//            auto word = PolarCoder::getWord(A, frozen, myMsg);
            auto gaussWord = gaussianChannel.Gauss(cW, N, K);
            begin = std::chrono::high_resolution_clock::now();
            auto decodedWord = decoder.decode(gaussWord, A, N, K);
//            auto decodedWord = decoderSC.decode(gaussWord, A, N, K);
            if (!isSystematic) {
                decodedWord = PolarCoder::encode(decodedWord, A, frozen, reversedIndexes);
            }
            auto dW = Message();
            for (size_t p = 0; p < N; p++) {
                if (A.count(p) == 1) {
                    dW.emplace_back(decodedWord[p]);
                }
            }
            end = std::chrono::high_resolution_clock::now();
            totalDecoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            if (compareWords(dW, myMsg) > 0) {
                if (isBER) {
                    e += compareWords(dW, myMsg);
                } else {
                    e += 1;
                }
            }
        }
        size_t totalAmount = 0;
        if (isBER) {
            totalAmount = wordsAmount * K;
        } else {
            totalAmount = wordsAmount;
        }
        std::cout << ii << " " << (double) e / totalAmount << std::endl;
    }

//    std::cout << "Total encoder time: " << totalEncoder.count() << " microseconds" << std::endl;
//    std::cout << "Total decoder time: " << totalDecoder.count() << " microseconds" << std::endl;

}