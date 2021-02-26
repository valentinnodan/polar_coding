#include <iostream>
#include <chrono>

#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"
#include "decoder/PolarDecoderBP.h"

int main() {

    double becProbability = 0.5;
    size_t N = 8;
    size_t K = 4;
    const size_t wordsAmount = 1000000;
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

    for (size_t i = 1; i < 10; i++) {
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

            begin = std::chrono::high_resolution_clock::now();
            auto decodedWord = decoder.decode(gaussWord, A, frozen, revPlace, N, K);
            end = std::chrono::high_resolution_clock::now();
            totalDecoder += std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            if (compareWords(decodedWord, word) > 0){
                e += 1;
            }
//            printWord(decodedWord);
        }
        std::cout << "Decoded word error:  " << (double) e / (wordsAmount) << " Eb_N0: " << (double) i
                  << std::endl;

    }

//    std::cout << "Total encoder time: " << totalEncoder.count() << " microseconds" << std::endl;
//    std::cout << "Total decoder time: " << totalDecoder.count() << " microseconds" << std::endl;

    return 0;
}
