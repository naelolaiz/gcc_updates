// description: consteval functions MUST be evaluated at compile time; constinit guarantees static-init at compile time.
// reference: https://en.cppreference.com/w/cpp/language/consteval

#include "support/demo.hpp"
#include <cassert>

consteval int square_at_compile_time(int n) { return n * n; }

constinit int g_value = square_at_compile_time(7);  // computed at compile time, mutable at runtime

int main() {
    demo::title("C++20 consteval");
    constexpr int x = square_at_compile_time(5);
    static_assert(x == 25);

    // The result of consteval can feed into a constexpr context and a runtime context.
    int y = square_at_compile_time(8);
    DEMO_ASSERT(y == 64);

    DEMO_ASSERT(g_value == 49);
    g_value = 100;          // constinit only constrains *initial* value
    DEMO_ASSERT(g_value == 100);

    return 0;
}
