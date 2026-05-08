// description: Hand-rolled minimal generator using <coroutine> primitives -- co_yield, promise_type, coroutine_handle.
// reference: https://en.cppreference.com/w/cpp/language/coroutines
// note: C++23 ships std::generator which supersedes hand-rolled versions; see cpp23_generator.cpp.

#include "support/demo.hpp"
#include <cassert>
#include <coroutine>
#include <vector>

template <typename T>
struct Generator {
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> h;
    explicit Generator(std::coroutine_handle<promise_type> handle) : h(handle) {}
    Generator(const Generator&) = delete;
    Generator(Generator&& other) noexcept : h(other.h) { other.h = {}; }
    ~Generator() { if (h) h.destroy(); }

    bool next() {
        h.resume();
        return !h.done();
    }
    T value() const { return h.promise().current_value; }
};

Generator<int> fibs(int count) {
    int a = 0, b = 1;
    for (int i = 0; i < count; ++i) {
        co_yield a;
        int next = a + b;
        a = b;
        b = next;
    }
}

int main() {
    demo::title("C++20 coroutine generator");
    std::vector<int> got;
    auto g = fibs(8);
    while (g.next()) got.push_back(g.value());
    // 0,1,1,2,3,5,8,13
    DEMO_ASSERT((got == std::vector<int>{0, 1, 1, 2, 3, 5, 8, 13}));
    return 0;
}
