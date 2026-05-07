// gcc-test: std=c++11 min-gcc=13 topic=threading experimental=false
// description: std::mutex + RAII locks (lock_guard, unique_lock); std::lock locks N mutexes deadlock-free.
// reference: https://en.cppreference.com/w/cpp/thread/mutex

#include "support/demo.hpp"
#include <cassert>
#include <mutex>
#include <thread>
#include <vector>

class Counter {
public:
    void bump() {
        std::lock_guard<std::mutex> lk(m_);
        ++value_;
    }
    int snapshot() const {
        std::lock_guard<std::mutex> lk(m_);
        return value_;
    }
private:
    mutable std::mutex m_;
    int value_ = 0;
};

int main() {
    demo::title("C++11 mutex lock");
    Counter c;

    std::vector<std::thread> ts;
    for (int i = 0; i < 4; ++i)
        ts.emplace_back([&c] { for (int j = 0; j < 1000; ++j) c.bump(); });
    for (auto& t : ts) t.join();
    DEMO_ASSERT(c.snapshot() == 4000);

    // unique_lock: like lock_guard but supports deferred locking and unlocking.
    std::mutex m;
    std::unique_lock<std::mutex> lk(m, std::defer_lock);
    lk.lock();
    DEMO_ASSERT(lk.owns_lock());
    lk.unlock();

    // std::lock acquires multiple mutexes without risking deadlock.
    std::mutex a, b;
    std::lock(a, b);
    std::lock_guard<std::mutex> ga(a, std::adopt_lock);
    std::lock_guard<std::mutex> gb(b, std::adopt_lock);
    return 0;
}
