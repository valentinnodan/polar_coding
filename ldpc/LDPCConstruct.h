#include <Matrix.h>
#include <cmath>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

inline std::pair<Matrix<size_t>,
        Matrix<size_t>> transformToSets() {
    std::ifstream in("tanner.txt");
    if (!in) {
        std::cout << "Failed to open file " << std::endl;
        exit(0);
    }
    std::string row, curr;
    getline(in, row);
    std::istringstream line(row);
    size_t r, c;
    line >> c;
    line >> r;
    r = c - r;
    auto H = Matrix<int>(r, c);
    Matrix<size_t> rows(r, c);
    Matrix<size_t> columns(c, r);
    for (size_t i = 0; i < c; i++) {
        getline(in, row);
        std::istringstream curr_line(row);
        curr_line >> columns[i][0];
        for (size_t j = 1; j <= columns[i][0]; j++) {
            curr_line >> columns[i][j];
        }
    }
    for (size_t i = 0; i < r; i++) {
        getline(in, row);
        std::istringstream curr_line(row);
        curr_line >> rows[i][0];
        for (size_t j = 1; j <= rows[i][0]; j++) {
            curr_line >> rows[i][j];
        }
    }
    in.close();
    return std::make_pair(rows, columns);
}

inline Matrix<int> transformToMatrix(std::pair<Matrix<size_t>,
        Matrix<size_t>> const &sets) {
    size_t r, c;
    c = sets.second.height;
    r = sets.first.height;
    auto H = Matrix<int>(r, c, std::numeric_limits<int>::infinity());
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 1; j <= sets.first[i][0]; j++) {
            size_t jj = sets.first[i][j];
            H[i][jj] = 1;
        }
    }
    return H;
}

inline void shiftedEye(Matrix<int> &H, size_t s, size_t t, size_t p, size_t amount) {
    amount %= p;
    for (size_t i = 0; i < amount; i++) {
        H[p - amount + i + s * p][i + t * p] = 1;
    }
    for (size_t i = 0; i < p - amount; i++) {
        H[i + s * p][amount + i + t * p] = 1;
    }

}

inline Matrix<int> constructTanner(size_t N, size_t K, size_t p) {
    auto H = Matrix<int>(p * N, p * K, 0);
    int a = 2;
    int b = 5;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < K; j++) {
            shiftedEye(H, i, j, p, static_cast<size_t>(pow(b, j) * pow(a, i)));
        }
    }
    for (size_t i = 0; i < p * K; i++) {
        for (size_t j = i; j < p * K; j++) {

        }
    }
    return H;
}
