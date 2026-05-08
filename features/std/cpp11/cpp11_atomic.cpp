// gcc-test: std=c++11 min-gcc=13 topic=threading experimental=false
// description: std::atomic<T> for lock-free (where supported) shared state; memory_order tunes synchronisation strength.
// reference: https://en.cppreference.com/w/cpp/atomic/atomic

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <thread>
#include <vector>

int main() {
    demo::title("C++11 atomic");
    std::atomic<int>  counter{0};
    std::atomic<bool> ready{false};

    std::thread waiter([&] {
        while (!ready.load(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
        // Once ready is true, counter's effects (released by setter) are visible.
        DEMO_ASSERT(counter.load(std::memory_order_relaxed) >= 0);
    });

    constexpr int kThreads = 8;
    constexpr int kIters   = 1000;
    std::vector<std::thread> ts;
    for (int i = 0; i < kThreads; ++i)
        ts.emplace_back([&] {
            for (int j = 0; j < kIters; ++j)
                counter.fetch_add(1, std::memory_order_relaxed);
        });
    for (auto& t : ts) t.join();

    ready.store(true, std::memory_order_release);
    waiter.join();

    DEMO_ASSERT(counter.load() == kThreads * kIters);

    // compare_exchange_weak loop: classic CAS pattern.
    std::atomic<int> v{0};
    int expected = 0;
    while (!v.compare_exchange_weak(expected, 42, std::memory_order_acq_rel)) {
        // expected updated by CAS to current value; loop until success.
    }
    DEMO_ASSERT(v.load() == 42);
    return 0;
}
