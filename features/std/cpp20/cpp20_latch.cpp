// gcc-test: std=c++20 min-gcc=14 topic=threading experimental=false
// description: std::latch is a one-shot countdown synchroniser; threads wait until it hits zero.
// reference: https://en.cppreference.com/w/cpp/thread/latch

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <latch>
#include <thread>
#include <vector>

int main() {
    demo::title("C++20 latch");
    constexpr int kWorkers = 8;
    std::latch start_gate(1);            // released by main once all workers spun up
    std::latch finish_gate(kWorkers);    // workers count down as they finish

    std::atomic<int> work_done{0};

    std::vector<std::jthread> workers;
    workers.reserve(kWorkers);
    for (int i = 0; i < kWorkers; ++i) {
        workers.emplace_back([&] {
            start_gate.wait();                       // block until main releases
            work_done.fetch_add(1, std::memory_order_relaxed);
            finish_gate.count_down();                // signal completion
        });
    }

    // Workers are all blocked until the gate is released.
    start_gate.count_down();   // == count_down(1)

    finish_gate.wait();
    DEMO_ASSERT(work_done.load() == kWorkers);
    return 0;
}
