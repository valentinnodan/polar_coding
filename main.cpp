#include <chrono>

#include "polar_utils.h"
#include "ldpc/ldpc_utils.h"

int main() {

//    size_t N = 256;
//    size_t K = 128;
//    const size_t wordsAmount = 1000;
//
//
//    runSimulation(N, K, wordsAmount, true,  true);
    runSimulationLDPC(10000, 6,  16);
    return 0;
}
