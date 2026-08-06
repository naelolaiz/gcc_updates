// description: std::philox4x32 is a counter-based random engine with reproducible streams and inexpensive independent subsequences for parallel workloads.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2075r6.pdf
// why: Parallel simulations need deterministic engines whose streams can be partitioned reliably.
// before: Shared stateful engines, manual stream splitting, or third-party counter-based RNGs.
// pitfall: Reproducible pseudorandom output is not cryptographically secure randomness.

#include "support/demo.hpp"
#include <random>

int main() {
    demo::title("C++26 philox engine");
    std::philox4x32 first(12345);
    std::philox4x32 second(12345);

    DEMO_ASSERT(first == second);
    for (int i = 0; i < 8; ++i) {
        DEMO_ASSERT(first() == second());
    }
    DEMO_ASSERT(first == second);
    return 0;
}
