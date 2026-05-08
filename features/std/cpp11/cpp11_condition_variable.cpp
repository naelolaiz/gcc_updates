// gcc-test: std=c++11 min-gcc=13 topic=threading experimental=false
// description: std::condition_variable: classic mutex+predicate wait/notify. Always pair wait() with a predicate to handle spurious wake-ups.
// reference: https://en.cppreference.com/w/cpp/thread/condition_variable

#include "support/demo.hpp"
#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

int main() {
    demo::title("C++11 condition variable");
    std::queue<int>          q;
    std::mutex               m;
    std::condition_variable  cv;
    bool                     done = false;

    constexpr int N = 100;
    std::vector<int> received;
    received.reserve(N);

    std::thread consumer([&] {
        for (;;) {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [&] { return !q.empty() || done; });
            while (!q.empty()) {
                received.push_back(q.front());
                q.pop();
            }
            if (done) break;
        }
    });

    for (int i = 0; i < N; ++i) {
        {
            std::lock_guard<std::mutex> lk(m);
            q.push(i);
        }
        cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lk(m);
        done = true;
    }
    cv.notify_one();

    consumer.join();
    DEMO_ASSERT((int)received.size() == N);
    return 0;
}
