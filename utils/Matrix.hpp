//
// Created by valentinnodan on 11/5/20.
//

template<typename T>
Matrix<T>::Matrix(size_t h, size_t w) : data(h * w), height(h), width(w) {
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &a, const Matrix<T> &b) {
    auto res = Matrix<T>(a.height * b.height, a.width * b.width);
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

template<typename T>
Matrix<T>::Matrix(size_t h, size_t w, std::vector<T> vals) : data(h * w), height(h), width(w) {
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            data[i * w + j] = vals[i * w + j];
        }
    }
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
void Matrix<T>::addRow(std::vector<T> v) {
    height += 1;
    for (T i: v) {
        data.push_back(i);
    }
}

template<typename T>
std::vector<T> Matrix<T>::dot(std::vector<T> u) {
    auto myRes = std::vector<T>(width, 0);
    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            myRes[i] += data[j * width + i] * u[j];
        }
        myRes[i] %= 2;
    }
    return myRes;
}

