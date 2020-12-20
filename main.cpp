#include <iostream>

#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"

int main() {

    double becProbability = 0.5;
    size_t N = 1024;
    size_t K = 512;
    const size_t wordsAmount = 1000;
    auto frozen = Message(N - K);

    auto cPC = PolarCodeConstruct(becProbability);
    auto A = cPC.construct(N, K);

    auto reversedIndexes = genReversedIndex(N);
    for (size_t i = 0; i < 11; i++) {
        auto gaussianChannel = Channel(i);
        const auto decoder = PolarDecoder(gaussianChannel);
        int e = 0;
        for (size_t j = 0; j < wordsAmount; j++) {
            auto myMsg = getRandomWord(K);
            const auto cW = PolarCoder::encode(myMsg, A, frozen, reversedIndexes);
//            std::cout << "encode" << std::endl;
            auto word = PolarCoder::getWord(A, frozen, myMsg);
            auto gaussWord = gaussianChannel.Gauss(cW, N, K);
            auto decodedWord = decoder.decode(gaussWord, A, frozen, reversedIndexes, N, K);
//            std::cout << "decode" << std::endl;
            e += compareWords(decodedWord, word);
        }
        std::cout << "Decoded word error:  " << (double) e / (wordsAmount * N) << " Eb_N0: " << (double) i / 2 << std::endl;
    }

    return 0;
}
