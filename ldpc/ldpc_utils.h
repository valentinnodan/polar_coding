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
    size_t K = 546;
    auto c = LDPCCoder();
    auto ss = transformToSets();
    std::cout << ss.first.height << std::endl;
    auto m = c.transformH(transformToMatrix(ss));

    for (size_t i = ebLeftBorder; i < ebRightBorder; i++) {
        auto gaussianChannel = Channel((double)i/2);
        const auto decoder = DecoderBP();
        int e = 0;
        int eBER = 0;
        int eWER = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            auto r = c.encode(myMsg, m);
            auto gaussWord = gaussianChannel.Gauss(r, N, K);
            auto l = std::vector<double>(N);
            auto s = gaussianChannel.getSigma(N, K);

            for (size_t i1 = 0; i1 < N; ++i1) {
                l[i1] = 2 * gaussWord[i1].symbol / s;
            }
            auto res = decoder.decode(N, N - K, N, l, ss.second, ss.first, 50);
//            auto res = decoder.decode(N - K, N, N, l, ss.first, ss.second, 100);
//            printWord(res);
//            printWord(r);
//            std::cout << std::endl;
            if (compareWords(res, r) > 0){
//                std::cout << compareWords(res, r) << std::endl;
                e += 1;
                eBER += compareWords(res, r);
                eWER += 1;
//                e += compareWords(res, r);
            }
        }
//        std::cout << std::endl;
        std::cout << "BER: " <<(double) i / 2 << " " <<  (double) eBER / (wordsAmount * N)
                  << std::endl;
        std::cout << "WER: " <<(double) i / 2 << " " <<  (double) eWER / (wordsAmount)
                  << std::endl;

    }
}
