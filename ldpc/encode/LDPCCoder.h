//
// Created by valentinnodan on 3/27/21.
//

#include <Matrix.h>
#pragma once


class LDPCCoder {
public:
    Matrix<int> transformH(Matrix<int> const & H);
    Message encode(Message const & x, Matrix<int> const & H) const;
private:
    std::vector<size_t> permutation;
    void swapRows(Matrix<int> & H, size_t r, size_t s) const;
    void swapColumns(Matrix<int> & H, size_t r, size_t s);
    void gaussOp(Matrix<int> & H, size_t i, size_t j) const;
};
