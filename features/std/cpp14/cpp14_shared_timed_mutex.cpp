// description: std::shared_timed_mutex + std::shared_lock (both C++14): many concurrent readers OR one exclusive writer, with try_lock_for deadlines; C++17's shared_mutex is this minus the timed API.
// reference: https://en.cppreference.com/w/cpp/thread/shared_timed_mutex

#include "support/demo.hpp"
#include <atomic>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

int main() {
    demo::title("C++14 shared_timed_mutex");

    std::shared_timed_mutex mtx;
    int shared_value = 0;

    // Writer: plain exclusive lock, like any mutex.
    {
        std::lock_guard<std::shared_timed_mutex> writer(mtx);
        shared_value = 42;
    }

    // All four readers hold the SHARED lock at the same time: each one
    // waits (inside the lock) until the others have arrived, which could
    // never finish if shared ownership weren't truly concurrent.
    constexpr int kReaders = 4;
    std::atomic<int> inside{0};
    std::atomic<bool> all_saw_42{true};
    std::vector<std::thread> readers;
    for (int r = 0; r < kReaders; ++r) {
        readers.emplace_back([&] {
            std::shared_lock<std::shared_timed_mutex> lock(mtx);
            ++inside;
            while (inside.load() < kReaders) std::this_thread::yield();
            if (shared_value != 42) all_saw_42 = false;
        });
    }
    for (auto& t : readers) t.join();
    DEMO_ASSERT(inside.load() == kReaders);
    DEMO_ASSERT(all_saw_42.load());
    demo::value("concurrent shared holders", inside.load());

    // The timed API: a writer can give up after a deadline instead of
    // blocking forever behind readers.
    std::atomic<bool> reader_holds{false};
    std::atomic<bool> release_reader{false};
    std::thread reader([&] {
        std::shared_lock<std::shared_timed_mutex> lock(mtx);
        reader_holds = true;
        while (!release_reader) std::this_thread::yield();
    });
    while (!reader_holds) std::this_thread::yield();
    DEMO_ASSERT(!mtx.try_lock_for(std::chrono::milliseconds(20)));
    release_reader = true;
    reader.join();
    DEMO_ASSERT(mtx.try_lock_for(std::chrono::milliseconds(20)));
    mtx.unlock();
    return 0;
}
