// description: std::atomic<T>::wait/notify_one/notify_all enable lock-free condition-variable-style blocking.
// reference: https://en.cppreference.com/w/cpp/atomic/atomic/wait

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <thread>

int main() {
    demo::title("C++20 atomic wait");
    std::atomic<int> state{0};
    std::atomic<int> woken{0};

    std::jthread waiter([&] {
        // wait() blocks while value == old. When notified and value differs,
        // it returns. Spurious wakeups are handled internally.
        state.wait(0);
        woken.store(state.load(), std::memory_order_relaxed);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    state.store(42, std::memory_order_release);
    state.notify_one();

    waiter.join();
    DEMO_ASSERT(woken.load() == 42);
    return 0;
}
