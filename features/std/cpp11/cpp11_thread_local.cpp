// description: thread_local gives each thread its own copy of a static-duration variable, initialised on that thread's first use -- per-thread counters and caches without locks.
// reference: https://en.cppreference.com/w/cpp/language/storage_duration

#include "support/demo.hpp"
#include <atomic>
#include <thread>
#include <vector>

namespace {

thread_local int call_count = 0;  // one independent instance PER THREAD

int bump() { return ++call_count; }

}  // namespace

int main() {
    demo::title("C++11 thread_local");

    // The main thread bumps its own copy three times.
    bump();
    bump();
    DEMO_ASSERT(bump() == 3);

    // Every worker starts from ITS OWN zero, unaffected by main's 3 --
    // and none of this needs a mutex, because nothing is shared.
    std::atomic<bool> all_saw_fresh{true};
    std::vector<std::thread> workers;
    for (int i = 0; i < 4; ++i) {
        workers.emplace_back([&] {
            if (bump() != 1 || bump() != 2) all_saw_fresh = false;
        });
    }
    for (auto& t : workers) t.join();
    DEMO_ASSERT(all_saw_fresh.load());

    // The workers never touched the main thread's copy.
    DEMO_ASSERT(call_count == 3);
    demo::value("main-thread count", call_count);
    return 0;
}
