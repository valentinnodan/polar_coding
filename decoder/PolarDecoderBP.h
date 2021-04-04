#pragma once

#include <vector>
#include <cstdint>
#include <set>
#include <Matrix.h>

#include "Symbol.h"
#include "../channel/Channel.h"
#include "PolarDecoder.h"

class PolarDecoderBP : public PolarDecoder {
public:
    explicit PolarDecoderBP(Channel const &m) : PolarDecoder(m) {
    }

    Message decode(Message const &y, std::set<size_t> const &indices, Message const &frozen,
                   std::vector<size_t> const &reversedIndexes, size_t N, size_t K, Matrix<int> const & H_LDPC_like) const;


private:

    const size_t ITER = 1000;
    void
    updateLR(Matrix<double> &l, Matrix<double> &r) const;


    static double f(double x, double y) ;

    void visualize(Matrix<double> const &l, std::string text) const;
};

