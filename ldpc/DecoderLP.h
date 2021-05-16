//
// Created by valentinnodan on 4/10/21.
//

#pragma once

#include <cstddef>
#include <Symbol.h>
#include <Matrix.h>

class DecoderLP {

    static constexpr size_t MAX_DEG = 14;

public:

    DecoderLP (size_t n, size_t r) : data(n, r), n(n), r(r) {}

    Message decode(size_t needed, const std::vector<double> &llr,
                   const Matrix<size_t> &Nv,
                   const Matrix<size_t> &Nc,
                   size_t iter);
    double alpha = 0.1;

private:

    struct data {
        std::vector<double> decode_x;
        std::vector<double> decode_v;
        std::vector<int> xl1;
        std::vector<double> xl2;
        std::vector<double> xl3;
        std::vector<double> xl4;
        Matrix<double> lMatrix;
        std::vector<double> sums;

        data(size_t n, size_t r) : decode_x(n), decode_v(MAX_DEG), xl1(MAX_DEG), xl2(MAX_DEG), xl3(MAX_DEG), xl4(MAX_DEG), lMatrix(r, n), sums(n) {}
    };

    data data;
    size_t n;
    size_t r;


    double penalize(double t) const;
    static double projectDot(double dot) ;
    std::vector<double> &projectPolytope(std::vector<double> const & v, size_t s) ;
    std::vector<double> &projectProbabilitySimplex(std::vector<double> const & v, size_t s);
    std::vector<double> &membershipTest(std::vector<double> const & vS, std::vector<int> & f, std::vector<double> const & v, size_t s) ;
    static int powSign(int f) ;
};
