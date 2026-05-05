// gcc-test: std=c++23 min-gcc=14 topic=templates experimental=false
// description: Deducing this lets a lambda recurse into itself without std::function or fixed-point combinator gymnastics.
// reference: https://en.cppreference.com/w/cpp/language/lambda#Lambda_capture

#include <cassert>

int main() {
    // Lambda whose explicit object parameter is itself -- no std::function.
    auto factorial = [](this auto self, int n) -> int {
        return n <= 1 ? 1 : n * self(n - 1);
    };
    assert(factorial(0) == 1);
    assert(factorial(5) == 120);
    assert(factorial(10) == 3628800);

    auto fib = [](this auto self, int n) -> int {
        return n < 2 ? n : self(n - 1) + self(n - 2);
    };
    assert(fib(0) == 0);
    assert(fib(1) == 1);
    assert(fib(10) == 55);
    return 0;
}
