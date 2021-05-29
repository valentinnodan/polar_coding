//
// Created by valentinnodan on 11/5/20.
//

#include <cstring>
#include "Symbol.h"
#include "Matrix.h"


template<typename T>
Matrix<T>::Matrix(size_t h, size_t w) : height(h), width(w), data(h * w) {
}

template<typename T1>
Matrix<T1> operator*(Matrix<T1> const &a, Matrix<T1> const &b) {
    auto res = Matrix<T1>(a.height * b.height, a.width * b.width);
    for (size_t i1 = 0; i1 < a.height; ++i1) {
        for (size_t j1 = 0; j1 < a.width; ++j1) {
            for (size_t i2 = 0; i2 < b.height; ++i2) {
                for (size_t j2 = 0; j2 < b.width; ++j2) {
                    res.data[(i2 * a.height + i1) * (a.width * b.width) + j2 * a.width + j1] = a[i1][j1] * b[i2][j2];
                }
            }
        }
    }
    return res;
}

template<typename T>
Matrix<T>::Matrix(size_t n) : height(n), width(n), data(n * n) {
    for (size_t i = 0; i < n; ++i) {
        data[i * n + i] = T(1);
    }
}

template<typename T>
std::pair<size_t, size_t> Matrix<T>::dims() const {
    return {height, width};
}

template<typename T>
T const *Matrix<T>::operator[](size_t i) const {
    assert(i < height);
    return &data[i * width];
}

template<typename T>
T *Matrix<T>::operator[](size_t i) {
    assert(i < height);
    return &data[i * width];
}

template<typename T>
std::ostream &operator<<(std::ostream &s, Matrix<T> const &m) {
    for (size_t i = 0; i < m.height; ++i) {
        for (size_t j = 0; j < m.width; ++j) {
            s << m[i][j] << ' ';
        }
        s << '\n';
    }
    return s;
}

template<typename T>
Matrix<T>::Matrix(size_t h, size_t w, std::vector<T> const &vals) : height(h), width(w), data(h * w) {
    std::copy(vals.begin(), vals.begin() + h * w, data.begin());
}

template<typename T>
std::vector<T> Matrix<T>::getRow(size_t i) {
    auto myRes = std::vector<T>();
    for (size_t j = 0; j < width; ++j) {
        myRes.push_back(data[i * width + j]);
    }
    return myRes;
}

template<typename T>
void Matrix<T>::addRow(std::vector<T> const &v) {
    height += 1;
    for (T const &i: v) {
        data.push_back(i);
    }
}

template<typename T>
std::vector<T> Matrix<T>::dot(std::vector<T> const &u) {
    auto myRes = std::vector<T>(width, T(0));
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            myRes[i] += data[j * width + i] * u[j];
        }
    }
    return myRes;
}

template<typename T>
Matrix<T> Matrix<T>::mult(Matrix<T> const &a, Matrix<T> const &b) {
    auto res = Matrix<T>(a.height, b.width);
    for (size_t j = 0; j < b.width; ++j) {
        for (size_t k = 0; k < a.height; ++k) {
            for (size_t i = 0; i < a.width; ++i) {
                res[k][j] += a[k][i] * b[i][j];
            }
        }
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() {
    auto res = Matrix<T>(this->width, this->height);
    for (size_t i = 0; i < this->width; i++) {
        for (size_t j = 0; j < this->height; j++) {
            res[i][j] = this->operator[](j)[i];
        }
    }
    return res;
}

template<typename T>
Matrix<T>::Matrix(size_t h, size_t w, T val) : height(h), width(w), data(h * w) {
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            data[i * w + j] = val;
        }
    }
}

template<typename T>
void Matrix<T>::zero() {
    if constexpr (std::is_trivially_default_constructible_v<T>)
        std::memset(data.data(), 0, data.size() * sizeof(T));
    else
        std::fill(data.begin(), data.end(), 0);
}
