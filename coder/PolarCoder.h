//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <cmath>
#include <vector>
#include <set>
#include "Symbol.h"
#include "Matrix.h"

class PolarCoder {
public:
    static Message encode(Message const &u, std::set<size_t> const &indices, Message const &frozen);

    static Matrix<Symbol> getGN(int n);

private:
    const static Matrix<Symbol> F;
    const static Matrix<Symbol> I2;

    static Matrix<Symbol> getFN(int n);

    static Matrix<Symbol> RN(Matrix<Symbol> const &m);

    static Matrix<Symbol> getBN(int n);

};

inline const Matrix<Symbol> PolarCoder::F =
        Matrix<Symbol>(2, 2, Message{SymbolConsts::ONE, SymbolConsts::ZERO,
                                     SymbolConsts::ONE, SymbolConsts::ONE});
inline const Matrix<Symbol> PolarCoder::I2 = Matrix<Symbol>(2);

