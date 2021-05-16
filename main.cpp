#include <chrono>
#include <future>

#include "polar_utils.h"
#include "ldpc/ldpc_utils.h"

int main() {

    size_t N = 256;
    size_t K = 128;
    const size_t wordsAmount = 100;
//
//
//    runSimulationLDPC(wordsAmount, 3, 15);
    auto cres = std::async(std::launch::async, runSimulation, N, K, wordsAmount, 0.1, true, true, true);
//    auto res0 = std::async(std::launch::async, runSimulation, N, K, wordsAmount, 0.125, true, true, true);
//    auto res1 = std::async(std::launch::async, runSimulation, N, K, wordsAmount, 0.05, true, true, true);
//    auto res2 = std::async(std::launch::async, runSimulation, N, K, wordsAmount, 0.075, true, true, true);
//    auto res3 = std::async(std::launch::async, runSimulation, N, K, wordsAmount, 0.2, true, true, true);

//    std::cout << bres.get() << std::endl;
    std::cout << cres.get() << std::endl;
//    std::cout << res0.get() << std::endl;
//    std::cout << res1.get() << std::endl;
//    std::cout << res2.get() << std::endl;
//    std::cout << res3.get() << std::endl;

    return 0;
}
