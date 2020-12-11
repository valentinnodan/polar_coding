#include <iostream>

#include "Matrix.h"
#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"

int main() {
    double E = 12;
    auto gaussianChannel = ChannelMatrix(E);
    auto cPC = PolarCodeConstruct(0.25);

    auto frozen = Message{SymbolConsts::ZERO, SymbolConsts::ZERO, SymbolConsts::ZERO, SymbolConsts::ZERO};
    const auto myMsg = Message{SymbolConsts::ONE, SymbolConsts::ONE, SymbolConsts::ZERO, SymbolConsts::ONE};
    const int k = myMsg.size();
    const int n = static_cast<uint64_t>(k) + frozen.size();

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
    for (int i = 0; i < n; ++i) {
        std::cout << cW[i];
    }
    std::cout << "\n";


    // GVM - Gauss without mistakes
    auto trueGaussWord = Message(n);
    for (int i = 0; i < n; ++i) {
        if (cW[i].symbol == 0) {
            trueGaussWord[i].symbol = -1;
        } else {
            trueGaussWord[i].symbol = 1;
        }
    }
    const auto decoder = PolarDecoder(gaussianChannel);

    for (int i = 0; i < 20 ; i++) {
        auto gaussWord = gaussianChannel.Gauss(cW, n, k);
        auto decodedWord = decoder.decode(gaussWord, A, frozen, n, k);
        std::cout << "\n";
        std::cout << "Decoded word: ";
        int e = 0;
        for (size_t j = 0; j < n; ++j) {
            double diff = decodedWord[j].symbol - word[j].symbol;
            std::cout << decodedWord[j].symbol;
            if (abs(diff) > 0.01) {
                e++;
            }
        }
        std::cout << " " << e;
    }

    return 0;
}
