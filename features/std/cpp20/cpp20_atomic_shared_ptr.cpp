// gcc-test: std=c++20 min-gcc=14 topic=threading experimental=false
// description: std::atomic<std::shared_ptr<T>> -- lock-free-style atomic load/store/exchange of shared ownership.
// reference: https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic2

#include <atomic>
#include <cassert>
#include <memory>
#include <thread>
#include <vector>

struct Snapshot {
    int generation;
    int payload;
};

int main() {
    std::atomic<std::shared_ptr<Snapshot>> current{
        std::make_shared<Snapshot>(0, 100)};

    constexpr int kReaders = 4;
    constexpr int kWrites = 200;

    std::atomic<bool> stop{false};
    std::atomic<int> reader_observations{0};

    std::vector<std::jthread> readers;
    for (int i = 0; i < kReaders; ++i) {
        readers.emplace_back([&] {
            while (!stop.load(std::memory_order_acquire)) {
                auto snap = current.load(std::memory_order_acquire);
                // Each reader can deref freely -- it owns its own shared_ptr.
                assert(snap->payload == 100 + snap->generation);
                reader_observations.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    // Wait until every reader has loaded at least once. Without this the writer
    // can finish all stores and signal stop before any reader is scheduled
    // (visible under heavy sanitizer instrumentation), making the
    // `reader_observations > 0` post-condition flaky.
    while (reader_observations.load(std::memory_order_relaxed) < kReaders) {
        std::this_thread::yield();
    }

    for (int g = 1; g <= kWrites; ++g) {
        current.store(std::make_shared<Snapshot>(g, 100 + g),
                      std::memory_order_release);
    }
    stop.store(true, std::memory_order_release);
    readers.clear();

    auto final_snap = current.load();
    assert(final_snap->generation == kWrites);
    assert(reader_observations.load() > 0);
    return 0;
}
