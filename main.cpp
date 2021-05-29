#include <chrono>
#include <future>

#include "polar_utils.h"
#include "ldpc/ldpc_utils.h"

int main() {

    size_t N = 256;
    size_t K = 128;
    const size_t wordsAmount = 100;

//    runSimulationLDPC(wordsAmount, 3, 15);
    for (size_t a = 175; a <= 225; a += 5) {
        for (size_t mu = 350; mu <= 390; mu += 5) {
            auto cres =  runSimulation(N, K, wordsAmount, (double)a/1000, (double) mu/100, true, true, true);
            std::cout << cres << std::endl;
        }
    }

//    auto cres =  runSimulation(N, K, wordsAmount, 0.22, 3.7, true, true, false);
//    std::cout << cres << std::endl;
//    runSimulationSC(N, K, wordsAmount, true);
    return 0;
}
