#include <iostream>

#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"

int main() {

    double becProbability = 0.5;
    size_t N = 8;
    size_t K = 4;
    auto frozen = Message(N - K);

    auto cPC = PolarCodeConstruct(becProbability);
    auto A = cPC.construct(N, K);

    for (int i = 0; i < 11; i++) {
        auto myMsg = getRandomWord(K);
        const auto cW = PolarCoder::encode(myMsg, A, frozen);
        auto word = PolarCoder::getWord(A, frozen, myMsg);
        auto gaussianChannel = Channel(i);
        const auto decoder = PolarDecoder(gaussianChannel);
        auto gaussWord = gaussianChannel.Gauss(cW, N, K);
        auto decodedWord = decoder.decode(gaussWord, A, frozen, N, K);
        std::cout << "Decoded word error: ";
        int e = 0;
        for (size_t j = 0; j < N; ++j) {
            double diff = decodedWord[j].symbol - word[j].symbol;
            if (std::abs(diff) > 0.1) {
                e++;
            }
        }
        std::cout << " " << e << " Eb_N0: " << i << std::endl;
    }

    return 0;
}
