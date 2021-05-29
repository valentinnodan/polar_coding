//
// Created by valentinnodan on 2/18/21.
//

#include <iostream>
#include "PolarDecoderBP.h"
#include "../ldpc/DecoderBP.h"
#include "Transform.h"


Message PolarDecoderBP::decode(const Message &y, const std::set<size_t> &indices) {
    size_t n = y.size();

    auto s = myChannel.getSigma(n, k);

    size_t step = n - indices.size();
    auto l = std::vector<double>(n * (log2(n) + 1) - step, 0);
    for (size_t i = 0; i < n; ++i) {
        l[i + n * log2(n) - step] = 2 * y[i].symbol / s;
    }
    auto decoderLDPC = DecoderBP(n * (log2(n) + 1) - step, n * log2(n));
    auto decodedWord = decoderLDPC.decode(n, l, pair.second, pair.first, 200);
    for (size_t p = 0; p < n; p++) {
        if (indices.count(p) == 0) {
            decodedWord[p].symbol = 0;
        }
    }
    return decodedWord;
}
