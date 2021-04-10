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
            Matrix<size_t>> const &pair) : PolarDecoder(m), pair(pair) {
    }

    Message decode(Message const &y, std::set<size_t> const &indices, size_t N, size_t K) const;

private:
    std::pair<Matrix<size_t>,
            Matrix<size_t>> const &pair;
};

