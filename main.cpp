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
    int n = 4;
    int k = 2;
    auto A = cPC.construct(n, k);
    std::cout << "A = ";
    for (auto x : A) {
        std::cout << x << " ";
    }
    std::cout << "\n";


    auto frozen = Message{SymbolConsts::ONE, SymbolConsts::ZERO};
    const auto cW = PolarCoder::encode(Message{SymbolConsts::ONE, SymbolConsts::ONE},
                                       A,
                                       frozen);
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
