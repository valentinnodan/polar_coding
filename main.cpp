#include <iostream>
#include <chrono>

#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"
#include "decoder/PolarDecoderBP.h"
#include "ldpc/encode/LDPCCoder.h"
#include "ldpc/LDPCConstruct.h"
#include "ldpc/DecoderBP.h"
#include "polar_utils.h"

int main() {

//    double becProbability = 0.5;
//    size_t N = 4;
//    size_t K = 1;
    const size_t wordsAmount = 100;


    runSimulation(0.5, 8, 4, 1000);

//    std::vector<int> rr = std::vector<int>(r.size());
//    for (size_t i = 0; i < r.size(); i++) {
//        rr[i] = r[i].symbol;
//    }
//    auto rrr = Matrix<int>::mult(transformToMatrix(transformToSets()), Matrix(672, 1, rr));
//    size_t a = 0;
//    for (size_t i = 0; i < m.height; i++) {
//        if (rrr[0][i] % 2 == 1) {
//            a++;
//        }
//    }
//    std::cout << a << std::endl;


//    size_t N = 672;
//    size_t K = 672 - 336;
//    auto c = LDPCCoder();
//    auto ss = transformToSets();
//    auto m = c.transformH(transformToMatrix(ss));
//
//    for (size_t i = 1; i < 101; i++) {
//        auto gaussianChannel = Channel((double)i/2);
//        const auto decoder = DecoderBP();
//        int e = 0;
//        for (size_t j = 0; j < wordsAmount; j++) {
//            auto myMsg = getRandomWord(K);
//            auto r = c.encode(myMsg, m);
//            auto gaussWord = gaussianChannel.Gauss(r, N, K);
////            printWord(gaussWord);
//            auto l = std::vector<double>(N);
//            auto s = gaussianChannel.getSigma(N, K);
//
//            for (size_t i1 = 0; i1 < N; ++i1) {
////                if (gaussWord[i1].symbol > 0) {
////                    l[i1] = 1e100;
////                } else {
////                    l[i1] = -1e100;
////                }
//                l[i1] = 2 * gaussWord[i1].symbol / s;
//            }
//            auto res = decoder.decode(N, N - K, N, l, ss.second, ss.first, 50);
////            printWord(r);
////            printWord(res);
////            std::cout << j << std::endl;
//            if (j % 1000 == 1) {
//                std::cout << j << std::endl;
//            }
//            if (compareWords(res, r) > 0){
//                e += 1;
//            }
////            printWord(decodedWord);
//        }
//        std::cout <<  (double) e / (wordsAmount) << " " << (double) i / 2
//                  << std::endl;
//
//    }
    return 0;
}
