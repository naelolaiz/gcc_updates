// description: std::shared_mutex (C++17) + std::shared_lock = many readers OR one writer; std::unique_lock for the writer.
// reference: https://en.cppreference.com/w/cpp/thread/shared_mutex

#include "support/demo.hpp"
#include <atomic>
#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

class CountingMap {
public:
    void set(int v) {
        std::unique_lock lk(m_);   // exclusive
        value_ = v;
    }
    int get() const {
        std::shared_lock lk(m_);   // shared
        return value_;
    }
private:
    mutable std::shared_mutex m_;
    int value_ = 0;
};

int main() {
    demo::title("C++17 shared mutex");
    CountingMap cm;
    cm.set(100);

    std::atomic<int> ok_reads{0};

    std::vector<std::thread> readers;
    for (int i = 0; i < 8; ++i) {
        readers.emplace_back([&] {
            for (int j = 0; j < 100; ++j) {
                if (cm.get() == 100) ok_reads.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& t : readers) t.join();
    DEMO_ASSERT(ok_reads.load() == 800);

    cm.set(7);
    DEMO_ASSERT(cm.get() == 7);
    return 0;
}
