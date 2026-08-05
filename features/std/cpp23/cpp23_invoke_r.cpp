// description: std::invoke_r<R>(f, args...) is std::invoke with the result converted to R -- pin the return type in generic code (widen int to double, or discard via R=void) without casting at every call site.
// reference: https://en.cppreference.com/w/cpp/utility/functional/invoke

#include "support/demo.hpp"
#include <functional>
#include <type_traits>

int half(int x) { return x / 2; }

struct Accumulator {
    int total = 0;
    int add(int x) { return total += x; }
};

int main() {
    demo::title("C++23 invoke_r");

    // The conversion is part of the call: int -> double, no cast at the site.
    auto d = std::invoke_r<double>(half, 5);
    static_assert(std::is_same_v<decltype(d), double>);
    DEMO_ASSERT(d == 2.0);

    // R = void discards the return value explicitly -- useful in generic
    // wrappers that must not trip [[nodiscard]].
    std::invoke_r<void>(half, 4);

    // Member pointers work exactly like with std::invoke.
    Accumulator acc;
    auto t = std::invoke_r<long>(&Accumulator::add, acc, 10);
    static_assert(std::is_same_v<decltype(t), long>);
    DEMO_ASSERT(t == 10L && acc.total == 10);
    demo::value("acc.total", acc.total);
    return 0;
}
