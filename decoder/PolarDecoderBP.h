#pragma once

#include <vector>
#include <cstdint>
#include <set>
#include <Matrix.h>

#include "Symbol.h"
#include "../channel/Channel.h"
#include "PolarDecoderSC.h"
#include "PolarDecoder.h"

class PolarDecoderBP : public PolarDecoder {
public:
    explicit PolarDecoderBP(Channel const &m, std::pair<Matrix<size_t>,
            Matrix<size_t>> const &pair, size_t n, size_t k) : PolarDecoder(m, n, k), pair(pair) {
    }

    Message decode(Message const &y, std::set<size_t> const &indices) override;

private:
    std::pair<Matrix<size_t>,
            Matrix<size_t>> const &pair;
};

