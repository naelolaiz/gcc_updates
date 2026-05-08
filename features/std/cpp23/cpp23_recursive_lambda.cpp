// description: Deducing this lets a lambda recurse into itself without std::function or fixed-point combinator gymnastics.
// reference: https://en.cppreference.com/w/cpp/language/lambda#Lambda_capture

#include "support/demo.hpp"
#include <cassert>

int main() {
    demo::title("C++23 recursive lambda");
    // Lambda whose explicit object parameter is itself -- no std::function.
    auto factorial = [](this auto self, int n) -> int {
        return n <= 1 ? 1 : n * self(n - 1);
    };
    DEMO_ASSERT(factorial(0) == 1);
    DEMO_ASSERT(factorial(5) == 120);
    DEMO_ASSERT(factorial(10) == 3628800);

    auto fib = [](this auto self, int n) -> int {
        return n < 2 ? n : self(n - 1) + self(n - 2);
    };
    DEMO_ASSERT(fib(0) == 0);
    DEMO_ASSERT(fib(1) == 1);
    DEMO_ASSERT(fib(10) == 55);
    return 0;
}
