// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'constexpr' allows functions and variables to be evaluated at compile time when their inputs are constant.
// reference: https://en.cppreference.com/w/cpp/language/constexpr

#include <cassert>

constexpr int square(int n) { return n * n; }

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);   // C++11 constexpr requires single return
}

constexpr int kAnswer = 6 * 7;

int main() {
    static_assert(square(5) == 25, "");
    static_assert(factorial(6) == 720, "");
    static_assert(kAnswer == 42, "");

    // Same function works at runtime too.
    int x = 10;
    assert(square(x) == 100);
    return 0;
}
