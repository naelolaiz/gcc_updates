// description: 'constexpr' allows functions and variables to be evaluated at compile time when their inputs are constant.
// reference: https://en.cppreference.com/w/cpp/language/constexpr

#include "support/demo.hpp"
#include <cassert>

constexpr int square(int n) { return n * n; }

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);   // C++11 constexpr requires single return
}

constexpr int kAnswer = 6 * 7;

int main() {
    demo::title("C++11 constexpr");
    static_assert(square(5) == 25, "");
    static_assert(factorial(6) == 720, "");
    static_assert(kAnswer == 42, "");

    // Same function works at runtime too.
    int x = 10;
    DEMO_ASSERT(square(x) == 100);
    return 0;
}
