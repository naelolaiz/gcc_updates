// description: std::thread launches a thread; .join() waits, .detach() abandons. Failing to do either before destruction calls std::terminate.
// reference: https://en.cppreference.com/w/cpp/thread/thread

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <thread>
#include <vector>

int main() {
    demo::title("C++11 thread");
    std::atomic<int> counter{0};

    auto worker = [&counter](int n) {
        for (int i = 0; i < n; ++i) counter.fetch_add(1, std::memory_order_relaxed);
    };

    std::vector<std::thread> ts;
    for (int i = 0; i < 4; ++i) ts.emplace_back(worker, 1000);
    for (auto& t : ts) t.join();

    DEMO_ASSERT(counter.load() == 4000);

    // hardware_concurrency() is the suggested level of parallelism (may return 0).
    unsigned hc = std::thread::hardware_concurrency();
    (void)hc;
    return 0;
}
