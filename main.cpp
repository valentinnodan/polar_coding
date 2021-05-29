#include <chrono>
#include <future>

#include "polar_utils.h"
#include "ldpc/ldpc_utils.h"

int main() {

    size_t N = 256;
    size_t K = 128;
    const size_t wordsAmount = 100;

//    runSimulationLDPC(wordsAmount, 3, 15);
    for (size_t a = 25; a <= 150; a += 25) {
        for (size_t mu = 275; mu <= 425; mu += 25) {
            auto cres =  runSimulation(N, K, wordsAmount, (double)a/100, (double) mu/100, true, true, true);
            std::cout << cres << std::endl;
        }
    }

    return 0;
}
