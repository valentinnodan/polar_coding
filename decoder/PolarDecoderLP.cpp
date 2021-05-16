//
// Created by valentinnodan on 4/12/21.
//

#include "PolarDecoderLP.h"
#include "../ldpc/DecoderLP.h"


Message PolarDecoderLP::decode(const Message &y, const std::set<size_t> &indices) {
    assert (y.size() == n);
    assert (indices.size() == n - k);

    auto s = myChannel.getSigma(n, k);
    size_t step = n - indices.size();
    auto &l = data.decode_l;
    double kk = n * log2(n);
    for (size_t i = 0; i < n; ++i) {
        l[i + kk - step] = 2 * y[i].symbol / s;
    }

    auto decodedWord = data.decoderLDPC.decode(n, l, pair.second, pair.first, 100);
    for (size_t p = 0; p < n; p++) {
        if (indices.count(p) == 0) {
            decodedWord[p].symbol = 0;
        }
    }
    return decodedWord;
}
