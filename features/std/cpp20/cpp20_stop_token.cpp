// description: std::stop_source / stop_token / stop_callback decouple cancellation request from the threads observing it.
// reference: https://en.cppreference.com/w/cpp/thread/stop_source

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <stop_token>
#include <thread>
#include <vector>

int main() {
    demo::title("C++20 stop token");
    std::stop_source src;
    std::atomic<int> callback_fired{0};
    std::atomic<int> observed{0};

    // Multiple stop_callbacks on the same source all fire when stop is requested.
    std::stop_callback cb1(src.get_token(), [&] { callback_fired.fetch_add(1); });
    std::stop_callback cb2(src.get_token(), [&] { callback_fired.fetch_add(10); });

    std::vector<std::jthread> workers;
    for (int i = 0; i < 4; ++i) {
        workers.emplace_back([token = src.get_token(), &observed]() {
            while (!token.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            observed.fetch_add(1, std::memory_order_relaxed);
        });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    bool first = src.request_stop();
    bool second = src.request_stop();   // already requested -> false

    DEMO_ASSERT(first);
    DEMO_ASSERT(!second);

    workers.clear();   // joins
    DEMO_ASSERT(observed.load() == 4);
    DEMO_ASSERT(callback_fired.load() == 11);
    return 0;
}
