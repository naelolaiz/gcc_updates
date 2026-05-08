// gcc-test: std=c++20 min-gcc=14 topic=threading experimental=false
// description: C++20 added test()/wait()/notify_one to std::atomic_flag, finally making it useful beyond test_and_set.
// reference: https://en.cppreference.com/w/cpp/atomic/atomic_flag

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <thread>

int main() {
    demo::title("C++20 atomic flag test");
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

    // test() (new in C++20) returns the current state without modifying it.
    DEMO_ASSERT(flag.test() == false);

    flag.test_and_set();
    DEMO_ASSERT(flag.test() == true);

    // Use it as a one-shot signal: waiter blocks until producer clears it.
    std::atomic<int> got{-1};
    std::jthread waiter([&] {
        flag.wait(true);                     // blocks while flag == true
        got.store(7, std::memory_order_release);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    flag.clear(std::memory_order_release);
    flag.notify_one();

    waiter.join();
    DEMO_ASSERT(got.load() == 7);
    return 0;
}
