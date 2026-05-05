// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: <random>: an engine (Mersenne, etc.) plus a distribution. Replaces srand/rand for serious code.
// reference: https://en.cppreference.com/w/cpp/numeric/random

#include <cassert>
#include <random>

int main() {
    std::mt19937                    eng(42);          // seeded engine
    std::uniform_int_distribution<> die(1, 6);
    std::normal_distribution<>      gauss(0.0, 1.0);

    int sum = 0;
    for (int i = 0; i < 1000; ++i) sum += die(eng);
    assert(sum >= 1000 && sum <= 6000);              // bounds
    // Mean of 1..6 is 3.5; expected sum ~3500. Loose check:
    assert(sum > 3000 && sum < 4000);

    double s2 = 0;
    for (int i = 0; i < 1000; ++i) s2 += gauss(eng);
    // Mean 0, std 1, n=1000 -> mean of sample mean ~0 with std ~0.0316.
    double mean = s2 / 1000.0;
    assert(mean > -0.2 && mean < 0.2);

    // Seeding with std::random_device is the typical "real" entropy source.
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_int_distribution<> coin(0, 1);
    int x = coin(e2);
    assert(x == 0 || x == 1);
    return 0;
}
