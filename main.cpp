#include <iostream>

#include "Matrix.h"
#include "coder/PolarCoder.h"
#include "construct/PolarCodeConstruct.h"
#include "decoder/PolarDecoder.h"

int main() {
    auto a = Matrix<int>(2, 3);
    auto b = Matrix<int>(3, 1);
    auto a_vals = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b_vals = std::vector<int>{7, 8, 9};
    const auto[h, w] = a.dims();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            a[i][j] = i * w + j + 1;
        }
    }
    const auto[h1, w1] = b.dims();
    for (size_t i = 0; i < h1; ++i) {
        for (size_t j = 0; j < w1; ++j) {
            b[i][j] = i * w1 + j + 1;
        }
    }

    double p = 0;
    double q = 0.5;
    auto channel = ChannelMatrix(p, q);
    auto cPC = PolarCodeConstruct(channel);

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
    for (size_t i = 0; i < n; ++i) {
        if (A.count(i) == 1) {
            std::cout << myMsg[myMsgInd++];
        } else {
            std::cout << frozen[frozenInd++];
        }
    }
    std::cout << "\n";
    std::cout << "Code word:    ";
    for (int i = 0; i < n; ++i) {
        std::cout << cW[i];
    }
    std::cout << "\n";


    const auto becWord = channel.BEC(cW);
    const auto decoder = PolarDecoder(channel);
    auto decodedWord = decoder.decode(becWord, A, frozen);
    std::cout << "Decoded word: ";
    for (size_t i = 0; i < n; ++i) {
        std::cout << decodedWord[i];
    }

    return 0;
}
