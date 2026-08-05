// description: Relaxed constexpr: constexpr functions may contain loops, branches, and local mutation -- C++11's single-return-statement restriction is gone, so real algorithms run at compile time.
// reference: https://en.cppreference.com/w/cpp/language/constexpr

#include "support/demo.hpp"
#include <cstdint>

// Local variables and loops -- both forbidden in a C++11 constexpr function,
// which had to be a single return statement (usually recursive).
constexpr std::uint64_t factorial(unsigned n) {
    std::uint64_t r = 1;
    for (unsigned i = 2; i <= n; ++i) r *= i;
    return r;
}

constexpr int fib(int n) {
    int a = 0, b = 1;
    while (n-- > 0) {
        int t = a + b;
        a = b;
        b = t;
    }
    return a;
}

constexpr bool is_prime(unsigned n) {
    if (n < 2) return false;
    for (unsigned d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

int main() {
    demo::title("C++14 relaxed constexpr");

    static_assert(factorial(10) == 3628800, "evaluated at compile time");
    static_assert(fib(10) == 55, "iterative, not recursive");
    static_assert(is_prime(97) && !is_prime(91), "");

    // The same functions still work with runtime arguments.
    unsigned n = 12;
    DEMO_ASSERT(factorial(n) == 479001600u);
    demo::value("factorial(12)", factorial(n));
    demo::value("fib(20)", fib(20));
    return 0;
}
