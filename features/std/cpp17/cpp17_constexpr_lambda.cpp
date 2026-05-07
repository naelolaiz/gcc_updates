// gcc-test: std=c++17 min-gcc=13 topic=language experimental=false
// description: Lambdas can be constexpr; their call operator participates in constant evaluation.
// reference: https://en.cppreference.com/w/cpp/language/lambda

#include <array>
#include <cassert>

int main() {
    constexpr auto square = [](int n) { return n * n; };
    static_assert(square(5) == 25);

    // Use a lambda inside a constexpr context to fill a compile-time array.
    constexpr auto first_five_squares = []() {
        std::array<int, 5> a{};
        for (int i = 0; i < 5; ++i) a[i] = (i + 1) * (i + 1);
        return a;
    }();
    static_assert(first_five_squares[0] == 1);
    static_assert(first_five_squares[4] == 25);

    // Runtime use of the same lambda still works.
    int x = 6;
    assert(square(x) == 36);
    return 0;
}
