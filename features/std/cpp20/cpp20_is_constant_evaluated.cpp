// description: std::is_constant_evaluated() lets one constexpr function take a compile-time-safe path during constant evaluation and a faster/library path at runtime (C++23's 'if consteval' supersedes the pattern).
// reference: https://en.cppreference.com/w/cpp/types/is_constant_evaluated

#include "support/demo.hpp"
#include <cmath>
#include <type_traits>

constexpr double my_abs(double x) {
    if (std::is_constant_evaluated()) {
        return x < 0 ? -x : x;  // allowed in a constant expression
    }
    return std::fabs(x);  // runtime: defer to the libm builtin
}

// Pitfall to remember: 'if constexpr (std::is_constant_evaluated())' is
// ALWAYS true -- the condition itself then becomes a manifestly constant
// expression. Use a plain 'if' (GCC even warns about the constexpr form).

int main() {
    demo::title("C++20 is_constant_evaluated");

    static_assert(my_abs(-2.5) == 2.5, "took the constant-evaluation path");

    double runtime_val = -3.5;
    DEMO_ASSERT(my_abs(runtime_val) == 3.5);
    demo::value("my_abs(-3.5)", my_abs(runtime_val));
    return 0;
}
