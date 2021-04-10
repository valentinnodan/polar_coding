#include <chrono>

#include "ldpc/LDPCConstruct.h"
#include "polar_utils.h"

int main() {

    double becProbability = 0.5;
    size_t N = 256;
    size_t K = 128;
    const size_t wordsAmount = 100;


    runSimulation(becProbability, N, K, wordsAmount, true, false);

    return 0;
}
