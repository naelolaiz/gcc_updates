// description: std::jthread auto-joins on destruction and supports cooperative cancellation via stop_token.
// since: GCC 10 (jthread); fully usable across GCC 14/15/16.
// reference: https://en.cppreference.com/w/cpp/thread/jthread
// why: Thread lifetime and cancellation can follow RAII instead of depending on manual join paths.
// before: std::thread required an explicit join or detach before every destruction path.
// pitfall: A stop request is cooperative; work that never checks its token may still block destruction.

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <stop_token>
#include <thread>

int main() {
    demo::title("C++20 jthread");
    std::atomic<int> ticks{0};

    {
        std::jthread worker([&](std::stop_token st) {
            while (!st.stop_requested()) {
                ticks.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        worker.request_stop();
        // jthread joins automatically when going out of scope; no explicit
        // .join() needed -- destructor calls request_stop() then join().
    }

    DEMO_ASSERT(ticks.load() > 0);
    return 0;
}
