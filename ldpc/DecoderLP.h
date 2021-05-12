//
// Created by valentinnodan on 4/10/21.
//

#pragma once

#include <cstddef>
#include <Symbol.h>
#include <Matrix.h>

//#define double long double

class DecoderLP {
public:
    Message decode(size_t n, size_t r, size_t needed, const std::vector<double> &llr,
                   const Matrix<size_t> &Nv,
                   const Matrix<size_t> &Nc,
                   size_t iter) const;
    double alpha = 0.1;

private:
    double penalize(double t) const;
    double projectDot(double dot) const;
    std::vector<double> projectPolytope(std::vector<double> const & v, size_t s) const;
    std::vector<double> projectProbabilitySimplex(std::vector<double> const & v) const;
    std::vector<double> membershipTest(std::vector<double> const & vS, std::vector<double> const & u, std::vector<double> const & v) const;
    int powSign(int f) const;
};
