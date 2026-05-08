// description: std::atomic_ref overlays atomic semantics on a non-atomic object you don't own.
// reference: https://en.cppreference.com/w/cpp/atomic/atomic_ref

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <numeric>
#include <thread>
#include <vector>

int main() {
    demo::title("C++20 atomic ref");
    // A plain int that we want to update atomically from many threads,
    // without changing its declared type (e.g. it might come from a third-party
    // struct, or from a contiguous numeric buffer used elsewhere).
    int counter = 0;

    constexpr int kThreads = 8;
    constexpr int kIters = 1000;

    std::vector<std::jthread> ts;
    ts.reserve(kThreads);
    for (int i = 0; i < kThreads; ++i) {
        ts.emplace_back([&] {
            std::atomic_ref ref(counter);
            for (int j = 0; j < kIters; ++j) {
                ref.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    ts.clear();

    DEMO_ASSERT(counter == kThreads * kIters);
    return 0;
}
