//
// Created by valentinnodan on 11/5/20.
//

#pragma once

#include <cstddef>
#include <vector>
#include <ostream>

template<typename T>
class Matrix {

public:
    Matrix() = default;

    explicit Matrix(size_t n);

    Matrix(size_t h, size_t w);

    Matrix(size_t h, size_t w, T val);

    Matrix(size_t h, size_t w, std::vector<T> const &vals);

    [[nodiscard]] std::pair<size_t, size_t> dims() const;

    template<typename T1>
    friend Matrix<T1> operator*(Matrix<T1> const &a, Matrix<T1> const &b);

    T const *operator[](size_t i) const;

    T *operator[](size_t i);

    template<typename T1>
    friend std::ostream &operator<<(std::ostream &s, Matrix<T1> const &m);

    std::vector<T> getRow(size_t i);

    void addRow(std::vector<T> const &v);

    std::vector<T> dot(std::vector<T> const &u);

    void zero();

    static Matrix<T> mult(Matrix<T> const &a, Matrix<T> const &b);

    size_t height{};
    size_t width{};

    Matrix<T> transpose();

private:
    std::vector<T> data;
};

#include "Matrix.hpp"