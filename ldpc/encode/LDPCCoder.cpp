//
// Created by valentinnodan on 3/27/21.
//

#include <polar_utils.h>
#include "LDPCCoder.h"

Matrix<int> LDPCCoder::transformH(const Matrix<int> &H) {
    this->permutation = std::vector<size_t>(H.width);
    for (size_t i = 0; i < H.width; i++) {
        this->permutation[i] = i;
    }
    Matrix<int> GaussH(H);

    size_t row = H.height;
    size_t column = H.width;
    int i = 0, j = 0;
    while (i < row && j < column) {
        if (GaussH[i][j] == 0) {
            for (size_t t = i + 1; t < row; ++t) {
                if (GaussH[t][j] == 1) {
                    swapRows(GaussH, i, t);
                    break;
                }
            }
        }

        if (GaussH[i][j] == 0) {
            for (size_t t = j + 1; t < H.width; ++t) {
                if (GaussH[i][t] == 1) {
                    swapColumns(GaussH, j, t);
                    break;
                }
            }
        }
        gaussOp(GaussH, i, j);
        i++;
        j++;
    }
    return GaussH;
}

void LDPCCoder::swapRows(Matrix<int> &H, size_t r, size_t s) const {
    for (size_t i = 0; i < H.width; i++) {
        int p = H[r][i];
        H[r][i] = H[s][i];
        H[s][i] = p;
    }
}

void LDPCCoder::gaussOp(Matrix<int> &H, size_t i, size_t j) const {
    for (size_t r = 0; r < H.height; r++) {
        if (r != i && H[r][j] == 1)
            for (size_t s = 0; s < H.width; s++) {
                if (s < j && H[r][s] != H[i][s] + H[r][s]) {
                    std::cout << "Strange" << std::endl;
                }
                H[r][s] += H[i][s];
                H[r][s] = H[r][s] % 2;
            }
    }
}

Message LDPCCoder::encode(Message const &x, const Matrix<int> &H) const {
    auto res = Message(H.width);
    auto res1 = Message(H.width);
    size_t k = x.size();
    for (size_t i = 0; i < H.height; i++) {
        for (size_t j = 0; j < k; j++) {
            res[i].symbol += x[j].symbol * H[i][j + H.height];
        }
        res[i].symbol = int(res[i].symbol) % 2;
    }
    for (size_t i = 0; i < k; i++) {
        res[H.height + i] = x[i];
    }
    for (size_t i = 0; i < H.width; i++) {
        res1[this->permutation[i]].symbol = res[i].symbol;
    }
    return res1;
}

void LDPCCoder::swapColumns(Matrix<int> &H, size_t r, size_t s) {
    size_t pp = this->permutation[r];
    this->permutation[r] = this->permutation[s];
    this->permutation[s] = pp;
    for (size_t i = 0; i < H.height; i++) {
        int p = H[i][r];
        H[i][r] = H[i][s];
        H[i][s] = p;
    }
}
