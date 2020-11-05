//
// Created by valentinnodan on 11/5/20.
//

template<typename T>
Matrix<T>::Matrix(size_t h, size_t w) : data(h * w), height(h), width(w) {
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &a, const Matrix<T> &b) {
    auto res = Matrix<T>(a.height * b.height, a.width * b.width);
    auto res_ptr = res.data.data();
    for (size_t i1 = 0; i1 < a.height; ++i1) {
        for (size_t j1 = 0; j1 < a.width; ++j1) {
            for (size_t i2 = 0; i2 < b.height; ++i2) {
                for (size_t j2 = 0; j2 < b.width; ++j2) {
                    *(res_ptr++) = a[i1][j1] * b[i2][j2];
                }
            }
        }
    }
    return res;
}

template<typename T>
Matrix<T>::Matrix(size_t n) : data(n * n), height(n), width(n) {
    for (size_t i = 0; i < n; ++i) {
        data[i * n + i] = 1;
    }
}

template<typename T>
std::pair<size_t, size_t> Matrix<T>::dims() {
    return {height, width};
}

template<typename T>
T const *Matrix<T>::operator[](size_t i) const {
    return &data[i * width];
}

template<typename T>
T *Matrix<T>::operator[](size_t i) {
    return &data[i * width];
}

template<typename T>
std::ostream &operator<<(std::ostream &s, const Matrix<T> &m) {
    for (size_t i = 0; i < m.height; ++i) {
        for (size_t j = 0; j < m.width; ++j) {
            s << m[i][j] << ' ';
        }
        s << '\n';
    }
    return s;
}

