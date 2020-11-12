#include <iostream>

#include "Matrix.h"
#include "coder/PolarCoder.h"

int main() {
    auto a = Matrix<int>(2, 3);
    auto b = Matrix<int>(3, 1);
    auto a_vals = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto b_vals = std::vector<int>{7, 8, 9};
    const auto[h, w] = a.dims();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            a[i][j] = i * w + j + 1;
        }
    }
    const auto[h1, w1] = b.dims();
    for (size_t i = 0; i < h1; ++i) {
        for (size_t j = 0; j < w1; ++j) {
            b[i][j] = i * w1 + j + 1;
        }
    }
    auto cW = PolarCoder::encode(std::vector<uint64_t>{1, 1}, std::set<size_t>{1, 3}, std::vector<uint64_t>{1, 0});
    std::cout << cW[0] << cW[1] << cW[2] << cW[3];
    return 0;
}
