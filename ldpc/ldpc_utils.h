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
#include "DecoderADMM_LP.h"

inline void runSimulationLDPC(size_t wordsAmount, size_t ebLeftBorder, size_t ebRightBorder){
//    size_t N = 672;
//    size_t K = 546;
    size_t N = 155;
    size_t K = 62;
    auto c = LDPCCoder();
    auto H = constructTanner(5, 3, 31);
    H = H.transpose();
    auto ss = DecoderBP::prepare(H);
//    auto ss = transformToSets();
    std::cout << ss.first.height << std::endl;
    auto m = c.transformH(H);
    std::vector<std::string> bers = std::vector<std::string>(ebRightBorder - ebLeftBorder);
    std::vector<std::string> fers = std::vector<std::string>(ebRightBorder - ebLeftBorder);
#pragma omp parallel for
    for (size_t i = ebLeftBorder; i < ebRightBorder; i++) {
        double ii = (double) i / 2;
        auto gaussianChannel = Channel(ii);
        auto s = gaussianChannel.getSigma(N, N - K);
        const auto decoder = DecoderLP();
        int e = 0;
        int eBER = 0;
        int eWER = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            auto r = c.encode(myMsg, m);
            auto gaussWord = gaussianChannel.Gauss(r, N, K);
            auto l = std::vector<double>(N);

            for (size_t i1 = 0; i1 < N; ++i1) {
                l[i1] = 2 * gaussWord[i1].symbol / s;
            }
//            auto res = decoder.decode(N, N - K, N, l, ss.second, ss.first, 60);
            auto res = decoder.decode(N, N-K, N, l, ss.first, ss.second, 60);
            if (compareWords(res, r) > 0){
//                printWord(r);
//                printWord(res);
//                std::cout << std::endl;
//                std::cout << compareWords(res, r) << std::endl;
                e += 1;
                eBER += compareWords(res, r);
                eWER += 1;
//                e += compareWords(res, r);
            }
        }
//        std::cout << std::endl;
//        std::ostringstream ssBER;
//        std::ostringstream ssFER;
//        ssBER << ii << " " << (double) eBER / (wordsAmount * K);
//        bers.push_back(ssBER.str());
//        ssFER << ii << " " << (double) eWER / wordsAmount;
//        fers.push_back(ssFER.str());
////        std::cout << "BER: " << ii << " " << (double) eBER / (wordsAmount * N)
////                  << std::endl;
////        std::cout << "WER: " << ii << " " << (double) eWER / (wordsAmount)
////                  << std::endl;
//
//    }
//    std::cout << "BER" << std::endl;
//    for (auto & ber : bers) {
//        std::cout << ber << std::endl;
//    }
//    std::cout << "FER" << std::endl;
//    for (auto & fer : fers) {
//        std::cout << fer << std::endl;
//    }
//
//    {

        std::ostringstream ssBER;
        std::ostringstream ssFER;
        ssBER << ii << " " << (double) eBER / (wordsAmount * N);
        bers[i - ebLeftBorder] = ssBER.str();
        ssFER << ii << " " << (double) eWER / wordsAmount;
        fers[i - ebLeftBorder] = ssFER.str();
    }
    std::ostringstream output;
    output << "BER" << std::endl;
    for (auto & ber : bers) {
        output << ber << std::endl;
    }
    output << "FER" << std::endl;
    for (auto & fer : fers) {
        output << fer << std::endl;
    }
    std::cout << output.str();
}
