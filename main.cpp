#include <iostream>

#include "Matrix.h"
#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"

int main() {

    auto cPC = PolarCodeConstruct(0.5);

    size_t N = 16;
    auto frozen = Message(N);
    auto myMsg = Message(N);
    for (size_t i = 0; i < N; i++) {
        frozen[i] = SymbolConsts::ZERO;
        myMsg[i] = SymbolConsts::ONE;
    }
    myMsg[3] = SymbolConsts::ZERO;
    myMsg[5] = SymbolConsts::ZERO;
//    myMsg[100] = SymbolConsts::ZERO;
//    myMsg[128] = SymbolConsts::ZERO;
//    myMsg[156] = SymbolConsts::ZERO;
//    auto frozen = Message{SymbolConsts::ZERO, SymbolConsts::ZERO, SymbolConsts::ZERO, SymbolConsts::ZERO};
//    const auto myMsg = Message{SymbolConsts::ONE, SymbolConsts::ONE, SymbolConsts::ZERO, SymbolConsts::ONE};
    const size_t k = myMsg.size();
    const size_t n = k + frozen.size();

    auto A = cPC.construct(n, k);
    std::cout << "A = ";
    for (auto x : A) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    const auto cW = PolarCoder::encode(
            myMsg,
            A,
            frozen);
    std::cout << "Word coded:   ";
    size_t myMsgInd = 0;
    size_t frozenInd = 0;
    auto word = Message(n);
    for (size_t i = 0; i < n; ++i) {
        if (A.count(i) == 1) {
            word[i] = myMsg[myMsgInd];
            std::cout << myMsg[myMsgInd++];
        } else {
            word[i] = frozen[frozenInd];
            std::cout << frozen[frozenInd++];
        }
    }
    std::cout << "\n";
    std::cout << "Code word:    ";
    for (size_t i = 0; i < n; ++i) {
        std::cout << cW[i];
    }
    std::cout << "\n";


    // GVM - Gauss without mistakes
    auto trueGaussWord = Message(n);
    for (size_t i = 0; i < n; ++i) {
        if (cW[i].symbol == 0) {
            trueGaussWord[i].symbol = -1;
        } else {
            trueGaussWord[i].symbol = 1;
        }
    }

    for (int i = 1; i < 2; i++) {
        auto gaussianChannel = Channel(i);
        const auto decoder = PolarDecoder(gaussianChannel);
        auto gaussWord = gaussianChannel.Gauss(cW, n, k);
        auto decodedWord = decoder.decode(gaussWord, A, frozen, n, k);
        std::cout << "\n";
        std::cout << "Decoded word error: ";
        int e = 0;
        for (size_t j = 0; j < n; ++j) {
            double diff = decodedWord[j].symbol - word[j].symbol;
            if (std::abs(diff) > 0.1) {
                e++;
            }
        }
        std::cout << " " << e;
    }

    return 0;
}
