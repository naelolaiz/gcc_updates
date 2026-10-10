// description: C++20 added template parameter lists on lambdas, default-constructible and assignable captureless lambdas, and the explicit [=, this] capture (implicitly capturing this through [=] is deprecated).
// reference: https://en.cppreference.com/w/cpp/language/lambda

#include "support/demo.hpp"
#include <cassert>
#include <type_traits>
#include <vector>

struct Counter {
    int n = 0;
    auto bump_by_value_self() {
        // [*this] (C++17) copies the object: the lambda owns a Counter and
        // stays safe even if the original dies.
        return [*this](int x) mutable {
            n += x;
            return n;
        };
    }
    auto bump_through_this() {
        // C++20 spells the by-reference capture of this explicitly next to
        // '='; relying on [=] to capture this implicitly is deprecated.
        return [=, this](int x) {
            n += x;
            return n;
        };
    }
};

int main() {
    demo::title("C++20 lambdas");
    // Templated lambda: pin the type of args without auto-deduction quirks.
    auto first_of = []<typename T>(const std::vector<T>& v) -> T {
        return v.front();
    };
    DEMO_ASSERT(first_of(std::vector<int>{4, 5, 6}) == 4);
    DEMO_ASSERT(first_of(std::vector<double>{1.5, 2.5}) == 1.5);

    // Stateless lambdas are now default-constructible and assignable.
    auto add = [](int a, int b) { return a + b; };
    decltype(add) add2;          // default ctor - C++20 feature
    add2 = add;                  // copy assign - also C++20
    DEMO_ASSERT(add2(3, 4) == 7);

    // *this capture survives the parent.
    Counter c{.n = 10};
    auto fn = c.bump_by_value_self();
    c.n = 9999;                  // doesn't affect the captured copy
    DEMO_ASSERT(fn(5) == 15);
    DEMO_ASSERT(fn(2) == 17);

    // [=, this] writes through to the live object.
    auto through = c.bump_through_this();
    DEMO_ASSERT(through(1) == 10000);
    DEMO_ASSERT(c.n == 10000);

    return 0;
}
