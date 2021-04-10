#include <chrono>

#include "ldpc/LDPCConstruct.h"
#include "polar_utils.h"

int main() {

    size_t N = 256;
    size_t K = 128;
    const size_t wordsAmount = 100;


    runSimulation(N, K, wordsAmount, true,  false);

    return 0;
}
