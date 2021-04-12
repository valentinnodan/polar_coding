//
// Created by valentinnodan on 4/9/21.
//

#pragma once

#include <polar_utils.h>
#include "encode/LDPCCoder.h"
#include "LDPCConstruct.h"
#include "../channel/Channel.h"
#include "DecoderBP.h"
#include "DecoderLP.h"

inline void runSimulationLDPC(size_t wordsAmount, size_t ebLeftBorder, size_t ebRightBorder){
    size_t N = 672;
    size_t K = 672 - 336;
    auto c = LDPCCoder();
    auto ss = transformToSets();
    auto m = c.transformH(transformToMatrix(ss));

    for (size_t i = ebLeftBorder; i < ebRightBorder; i++) {
        auto gaussianChannel = Channel((double)i/2);
        const auto decoder = DecoderLP();
        int e = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            auto r = c.encode(myMsg, m);
            auto gaussWord = gaussianChannel.Gauss(r, N, K);
            auto l = std::vector<double>(N);
            auto s = gaussianChannel.getSigma(N, K);

            for (size_t i1 = 0; i1 < N; ++i1) {
                l[i1] = 2 * gaussWord[i1].symbol / s;
            }
//            auto res = decoder.decode(N, N - K, N, l, ss.second, ss.first, 100);
            auto res = decoder.decode(N - K, N, N, l, ss.first, ss.second, 200);
//            printWord(res);
//            printWord(r);
//            std::cout << std::endl;
            if (compareWords(res, r) > 0){
//                std::cout << compareWords(res, r) << " ";
//                e += 1;
                e += compareWords(res, r);
            }
        }
//        std::cout << std::endl;
        std::cout <<  (double) e / (wordsAmount * N) << " " << (double) i / 2
                  << std::endl;

    }
}
