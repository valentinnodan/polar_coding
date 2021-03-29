#include <Matrix.h>
#include <cmath>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

inline std::pair<Matrix<size_t>,
        Matrix<size_t>> transformToSets() {
    std::ifstream in("ldpc/672_336.txt");
    if (!in) {
        std::cout << "Failed to open file " << std::endl;
        exit(0);
    }
    std::string row, number, curr;
    getline(in, row);
    std::istringstream line(row);
    line >> number;
    size_t r, c;
    c = stoi(number);
    line >> number;
    r = stoi(number);
    auto H = Matrix<int>(r, c);
    Matrix<size_t> rows(r, c);
    Matrix<size_t> columns(c, r);
    for (size_t i = 0; i < c; i++) {
        getline(in, row);
        std::istringstream curr_line(row);
        curr_line >> number;
        columns[i][0] = stoi(number);
        for (size_t j = 1; j <= stoi(number); j++) {
            curr_line >> curr;
            columns[i][j] = stoi(curr);
        }
    }
    for (size_t i = 0; i < r; i++) {
        getline(in, row);
        std::istringstream curr_line(row);
        curr_line >> number;
        rows[i][0] = stoi(number);
        for (size_t j = 1; j <= stoi(number); j++) {
            curr_line >> curr;
            rows[i][j] = stoi(curr);
        }
    }
    in.close();
    return std::pair<Matrix<size_t>,
            Matrix<size_t>>{rows, columns};
}

inline Matrix<int> transformToMatrix (std::pair<Matrix<size_t>,
        Matrix<size_t>> const & sets) {
    std::ifstream in("ldpc/672_336.txt");
    if (!in) {
        std::cout << "Failed to open file" << std::endl;
        exit(0);
    }
    std::string row, number, curr;
    getline(in, row);
    std::istringstream line(row);
    line >> number;
    size_t r, c;
    c = stoi(number);
    line >> number;
    r = stoi(number);
    in.close();
    auto H = Matrix<int>(r, c, 0);
    for (size_t i = 0; i < r; i++) {
        for (size_t j = 1; j <= sets.first[i][0]; j++) {
            size_t jj = sets.first[i][j];
            H[i][jj] = 1;
        }
    }
    return H;
}
