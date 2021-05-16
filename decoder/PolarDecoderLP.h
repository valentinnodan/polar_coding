//
// Created by valentinnodan on 4/12/21.
//

#pragma once

#include "PolarDecoder.h"
#include <Matrix.h>
#include <ldpc/DecoderLP.h>

class PolarDecoderLP : PolarDecoder {

public:
    explicit PolarDecoderLP(Channel const &m, std::pair<Matrix<size_t>,
            Matrix<size_t>> const &pair, size_t n, size_t k) : PolarDecoder(m, n, k),
                                                               data(alpha, n, k),
                                                               pair(pair) {
    }


    Message decode(Message const &y, std::set<size_t> const &indices);

    void setAlpha(double alpha_new) {
        alpha = alpha_new;
        data.decoderLDPC.alpha = alpha;
    }

private:
    double alpha = 0.1;

    struct data {
        std::vector<double> decode_l;
        DecoderLP decoderLDPC;

        explicit data(double alpha, size_t n, size_t k) : decode_l(n * (log2(n) + 1) - k), decoderLDPC(n * (log2(n) + 1) - k, n * log2(n)) {
            decoderLDPC.alpha = alpha;
        }
    };

    data data;
    std::pair<Matrix<size_t>,
            Matrix<size_t>> const &pair;
};
