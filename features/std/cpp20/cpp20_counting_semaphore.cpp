// gcc-test: std=c++20 min-gcc=14 topic=threading experimental=false
// description: std::counting_semaphore caps concurrent access to a fixed number of "permits".
// reference: https://en.cppreference.com/w/cpp/thread/counting_semaphore

#include <atomic>
#include <cassert>
#include <semaphore>
#include <thread>
#include <vector>

int main() {
    constexpr int kPermits = 3;
    constexpr int kWorkers = 12;

    std::counting_semaphore<kPermits> sem(kPermits);

    std::atomic<int> in_flight{0};
    std::atomic<int> peak{0};
    std::atomic<int> finished{0};

    std::vector<std::jthread> workers;
    workers.reserve(kWorkers);
    for (int i = 0; i < kWorkers; ++i) {
        workers.emplace_back([&] {
            sem.acquire();
            int now = in_flight.fetch_add(1, std::memory_order_acq_rel) + 1;
            int prev_peak = peak.load(std::memory_order_relaxed);
            while (now > prev_peak &&
                   !peak.compare_exchange_weak(prev_peak, now,
                                                std::memory_order_relaxed)) {}

            // Pretend work.
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

            in_flight.fetch_sub(1, std::memory_order_acq_rel);
            finished.fetch_add(1, std::memory_order_relaxed);
            sem.release();
        });
    }
    workers.clear();

    assert(finished.load() == kWorkers);
    assert(peak.load() <= kPermits);   // semaphore upheld its cap
    assert(peak.load() >= 1);
    return 0;
}
