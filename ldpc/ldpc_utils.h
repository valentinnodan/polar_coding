//
// Created by valentinnodan on 4/9/21.
//

#pragma once

#include "polar_utils.h"
#include "encode/LDPCCoder.h"
#include "LDPCConstruct.h"
#include "channel/Channel.h"
#include "DecoderBP.h"
#include "DecoderLP.h"

inline void runSimulationLDPC(size_t wordsAmount, size_t ebLeftBorder, size_t ebRightBorder) {
    auto c = LDPCCoder();
//    auto H = constructTanner(5, 3, 31);
//    auto H = Matrix<int>(N, K, std::vector<int>{0, 0, 0, 1,
//                                               0, 0, 1, 1,
//                                               0, 1, 0, 1,
//                                               0, 1, 1, 1,
//                                               1, 0, 0, 1,
//                                               1, 0, 1, 1,
//                                               1, 1, 0, 1,
//                                               1, 1, 1, 1});
//    H = H.transpose();
//    std::cout << H << std::endl;
//    return;
//    auto ss = DecoderBP::prepare(H);
    auto ss = transformToSets();
    size_t N = ss.second.height;
    size_t K = N - ss.first.height;
    auto H = transformToMatrix(ss);
//    H = H.transpose();
    std::cout << ss.first.height << " " << ss.second.height << std::endl;
    auto m = c.transformH(H);
    std::vector<std::string> bers = std::vector<std::string>(ebRightBorder - ebLeftBorder);
    std::vector<std::string> fers = std::vector<std::string>(ebRightBorder - ebLeftBorder);
    std::vector<std::string> bersBP = std::vector<std::string>(ebRightBorder - ebLeftBorder);
    std::vector<std::string> fersBP = std::vector<std::string>(ebRightBorder - ebLeftBorder);
#pragma omp parallel for
    for (size_t i = ebLeftBorder; i < ebRightBorder; i++) {
        double ii = (double) i / 2;
        auto gaussianChannel = Channel(ii);
        auto s = gaussianChannel.getSigma(N, K);
        auto decoder = DecoderLP(N, N-K);
        const auto decoderBP = DecoderBP(N, N - K);
        int e = 0;
        int eBER = 0;
        int eWER = 0;
        int eBERBP = 0;
        int eWERBP = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            auto r = c.encode(myMsg, m);
            auto gaussWord = gaussianChannel.Gauss(r, N, K);
            auto l = std::vector<double>(N);

            for (size_t i1 = 0; i1 < N; ++i1) {
                l[i1] = 2 * gaussWord[i1].symbol / s;
            }
            auto res = decoder.decode(N, l, ss.second, ss.first, 60);
            auto resBP = decoderBP.decode( N, l, ss.second, ss.first, 60);
            if (compareWords(res, r) > 0) {
                e += 1;
                eBER += compareWords(res, r);
                eWER += 1;
            }
            if (compareWords(resBP, r) > 0) {
                eBERBP += compareWords(resBP, r);
                eWERBP += 1;
            }
        }
        std::ostringstream ssBER;
        std::ostringstream ssFER;
        ssBER << ii << " " << (double) eBER / (wordsAmount * N);
        bers[i - ebLeftBorder] = ssBER.str();
        ssFER << ii << " " << (double) eWER / wordsAmount;
        fers[i - ebLeftBorder] = ssFER.str();
        std::ostringstream ssBERBP;
        std::ostringstream ssFERBP;
        ssBERBP << ii << " " << (double) eBERBP / (wordsAmount * N);
        bersBP[i - ebLeftBorder] = ssBERBP.str();
        ssFERBP << ii << " " << (double) eWERBP / wordsAmount;
        fersBP[i - ebLeftBorder] = ssFERBP.str();
    }
    std::ostringstream output;
    output << "BER" << std::endl;
    for (auto &ber : bers) {
        output << ber << std::endl;
    }
    output << "FER" << std::endl;
    for (auto &fer : fers) {
        output << fer << std::endl;
    }
    output << "BERBP" << std::endl;
    for (auto &ber : bersBP) {
        output << ber << std::endl;
    }
    output << "FERBP" << std::endl;
    for (auto &fer : fersBP) {
        output << fer << std::endl;
    }
    std::cout << output.str();
}
