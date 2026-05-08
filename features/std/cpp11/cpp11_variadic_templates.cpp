// gcc-test: std=c++11 min-gcc=13 topic=templates experimental=false
// description: Variadic templates take a parameter pack 'Args...' that you expand with '...' and recurse over.
// reference: https://en.cppreference.com/w/cpp/language/parameter_pack

#include "support/demo.hpp"
#include <cassert>
#include <string>

// Recursive sum: pop one arg, recurse on the tail.
int sum_one() { return 0; }
template <typename T, typename... Rest>
T sum_one(T head, Rest... rest) {
    return head + sum_one(rest...);
}

// sizeof... returns the count of types/values in a pack.
template <typename... Args>
constexpr std::size_t arg_count(Args...) { return sizeof...(Args); }

// Pack expansion in an initializer list (C++11 idiom; C++17 fold makes this nicer).
template <typename... Args>
void touch_each(Args&... args) {
    int dummy[] = {(args = args + 1, 0)...};
    (void)dummy;
}

int main() {
    demo::title("C++11 variadic templates");
    DEMO_ASSERT(sum_one(1, 2, 3, 4) == 10);
    DEMO_ASSERT(sum_one(1.5, 2.5) == 4.0);

    static_assert(arg_count(1, 2, 3) == 3, "");

    int a = 10, b = 20, c = 30;
    touch_each(a, b, c);
    DEMO_ASSERT(a == 11 && b == 21 && c == 31);
    return 0;
}
