//
// Created by valentinnodan on 4/12/21.
//

#include "PolarDecoderLP.h"
#include "../ldpc/DecoderLP.h"

Message PolarDecoderLP::decode(const Message &y, const std::set<size_t> &indices, size_t N, size_t K) const {
    size_t n = y.size();

    auto s = myChannel.getSigma(N, K);
    size_t step = n - indices.size();
    auto l = std::vector<double>(n * (log2(n) + 1) - step, 0);
    double kk = n * log2(n);
    for (size_t i = 0; i < n; ++i) {
        l[i + kk - step] = 2 * y[i].symbol / s;
    }
    auto decoderLDPC = DecoderLP();
    decoderLDPC.alpha = alpha;
    auto nn = n * (log2(n) + 1);
    
    auto decodedWord = decoderLDPC.decode(nn - step, kk, n, l, pair.second, pair.first, 50);
    for (size_t p = 0; p < n; p++) {
        if (indices.count(p) == 0) {
            decodedWord[p].symbol = 0;
        }
    }
    return decodedWord;
}
