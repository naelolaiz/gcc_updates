// gcc-test: std=c++11 min-gcc=13 topic=threading experimental=false
// description: std::async runs work and returns a std::future; std::promise + std::packaged_task are the lower-level building blocks.
// reference: https://en.cppreference.com/w/cpp/thread/async

#include <cassert>
#include <future>
#include <thread>

int slow_square(int n) { return n * n; }

int main() {
    // Run async, possibly on another thread; future delivers the result.
    auto f = std::async(std::launch::async, slow_square, 7);
    assert(f.get() == 49);

    // Lambda + lazy launch.
    auto g = std::async(std::launch::deferred, [] { return 42; });
    assert(g.get() == 42);

    // std::promise + std::future: producer/consumer one-shot.
    std::promise<int> p;
    std::future<int>  fp = p.get_future();
    std::thread producer([&p] { p.set_value(123); });
    assert(fp.get() == 123);
    producer.join();

    // std::packaged_task wraps a callable as a future-producing task you can move around.
    std::packaged_task<int(int, int)> task([](int a, int b) { return a + b; });
    std::future<int> ft = task.get_future();
    std::thread t(std::move(task), 4, 5);
    assert(ft.get() == 9);
    t.join();
    return 0;
}
