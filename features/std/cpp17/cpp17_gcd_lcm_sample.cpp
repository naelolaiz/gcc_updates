// description: C++17 added std::gcd / std::lcm to <numeric> and std::sample to <algorithm> -- standardised greatest-common-divisor / least-common-multiple and uniform random sampling without replacement.
// reference: https://en.cppreference.com/w/cpp/numeric/gcd

#include "support/demo.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

int main() {
    demo::title("C++17 gcd / lcm / sample");

    static_assert(std::gcd(12, 18) == 6);
    static_assert(std::lcm(12, 18) == 36);
    static_assert(std::gcd(0, 7)   == 7);  // gcd(0, n) = n

    DEMO_ASSERT(std::gcd(15, 25) == 5);
    DEMO_ASSERT(std::lcm(4, 6) == 12);

    // std::sample picks N elements uniformly at random WITHOUT replacement.
    // Pre-C++17 you'd shuffle + take prefix, which is O(n) memory you don't
    // need; sample is O(N) memory and a single pass.
    std::vector<int> population(20);
    std::iota(population.begin(), population.end(), 1);

    std::vector<int> picked;
    std::mt19937 rng(0xBEEF);
    std::sample(population.begin(), population.end(),
                std::back_inserter(picked), 5, rng);

    DEMO_ASSERT(picked.size() == 5);
    // All picks are unique and from the population.
    for (int x : picked) {
        DEMO_ASSERT(x >= 1 && x <= 20);
    }
    demo::range("5 of 20", picked);
    return 0;
}
