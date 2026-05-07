// gcc-test: std=c++20 min-gcc=14 topic=threading experimental=false
// description: std::barrier is reusable across phases and runs an optional completion function between phases.
// reference: https://en.cppreference.com/w/cpp/thread/barrier

#include "support/demo.hpp"
#include <atomic>
#include <barrier>
#include <cassert>
#include <thread>
#include <vector>

int main() {
    demo::title("C++20 barrier");
    constexpr int kWorkers = 4;
    constexpr int kPhases = 3;

    std::atomic<int> phase{0};
    std::atomic<int> completion_runs{0};

    auto on_phase_done = [&]() noexcept {
        // Runs exactly once per phase, after every worker has arrived.
        phase.fetch_add(1, std::memory_order_release);
        completion_runs.fetch_add(1, std::memory_order_relaxed);
    };

    std::barrier sync(kWorkers, on_phase_done);

    std::atomic<int> work_per_phase[kPhases]{};

    std::vector<std::jthread> workers;
    workers.reserve(kWorkers);
    for (int i = 0; i < kWorkers; ++i) {
        workers.emplace_back([&] {
            for (int p = 0; p < kPhases; ++p) {
                work_per_phase[p].fetch_add(1, std::memory_order_relaxed);
                sync.arrive_and_wait();   // blocks until kWorkers arrived
            }
        });
    }
    workers.clear();

    for (int p = 0; p < kPhases; ++p) {
        DEMO_ASSERT(work_per_phase[p].load() == kWorkers);
    }
    DEMO_ASSERT(completion_runs.load() == kPhases);
    return 0;
}
