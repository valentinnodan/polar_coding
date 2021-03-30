#include <iostream>
#include <chrono>

#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"
#include "decoder/PolarDecoderBP.h"
#include "ldpc/encode/LDPCCoder.h"
#include "ldpc/LDPCConstruct.h"
#include "ldpc/DecoderBP.h"

int main() {

//    double becProbability = 0.5;
//    size_t N = 4;
//    size_t K = 1;
    const size_t wordsAmount = 1000000;
//    auto frozen = Message(N - K);
//
//    auto cPC = PolarCodeConstruct(becProbability);
//    auto A = cPC.construct(N, K);
//
//    auto reversedIndexes = genReversedIndex(N);
//    auto revPlace = std::vector<size_t>(N);
//    for (size_t i = 0; i < N; i++) {
//        revPlace[reversedIndexes[i]] = i;
//    }
//
//    std::chrono::microseconds totalEncoder;
//    std::chrono::microseconds totalDecoder;
//
//    for (size_t i = 1; i < 2; i++) {
//        auto gaussianChannel = Channel(i);
//        const auto decoder = PolarDecoderBP(gaussianChannel);
//        int e = 0;
//        for (size_t j = 0; j < wordsAmount; j++) {
//            auto myMsg = getRandomWord(K);
//            auto begin = std::chrono::high_resolution_clock::now();
//            const auto cW = PolarCoder::encode(myMsg, A, frozen, reversedIndexes);
//            auto end = std::chrono::high_resolution_clock::now();
//            totalEncoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
//
//            auto word = PolarCoder::getWord(A, frozen, myMsg);
//            printWord(word);
//            auto gaussWord = gaussianChannel.Gauss(cW, N, K);
////            printWord(gaussWord);
//
//            begin = std::chrono::high_resolution_clock::now();
//            auto decodedWord = decoder.decode(gaussWord, A, frozen, revPlace, N, K);
//            printWord(decodedWord);
//            end = std::chrono::high_resolution_clock::now();
//            totalDecoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
//            if (compareWords(decodedWord, word) > 0){
//                e += 1;
//            }
//            printWord(decodedWord);
//        }
//        std::cout << "Decoded word error:  " << (double) e / (wordsAmount) << " Eb_N0: " << (double) i
//                  << std::endl;
//
//    }

//    std::cout << "Total encoder time: " << totalEncoder.count() << " microseconds" << std::endl;
//    std::cout << "Total decoder time: " << totalDecoder.count() << " microseconds" << std::endl;


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

    double eX = exp(40);
    std::cout << log((eX + 1) / (eX - 1)) << std::endl;

    size_t N = 672;
    size_t K = 672 - 336;
    auto c = LDPCCoder();
    auto ss = transformToSets();
    auto m = c.transformH(transformToMatrix(ss));

    for (size_t i = 1; i < 10; i++) {
        auto gaussianChannel = Channel(i);
        const auto decoder = DecoderBP();
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
            auto res = decoder.decode(N, N - K, N, l, ss.second, ss.first, 20);
//            printWord(r);
//            printWord(res);
//            std::cout << j << std::endl;
//            if (j % 100 == 0) {
//                std::cout << j << std::endl;
//            }
            if (compareWords(res, r) > 0){
                e += 1;
            }
//            printWord(decodedWord);
        }
        std::cout <<  (double) e / (wordsAmount) << " " << (double) i
                  << std::endl;

    }
    return 0;
}
