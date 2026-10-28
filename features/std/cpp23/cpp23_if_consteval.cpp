// description: 'if consteval' is the standardised, simpler replacement for std::is_constant_evaluated().
// reference: https://en.cppreference.com/w/cpp/language/if#Consteval_if
// why: Branching between compile-time and runtime evaluation becomes a dedicated language construct that permits immediate (consteval) calls in the true branch.
// before: C++20 used 'if (std::is_constant_evaluated())', which was easy to misuse with 'if constexpr' and could not call consteval functions in a runtime-capable constexpr function.
// pitfall: 'if consteval' requires a compound statement '{ ... }' (no bare single statement) and cannot have a condition expression in parentheses.

#include "support/demo.hpp"
#include <cassert>

constexpr int magic(int n) {
    if consteval {
        // We're in a constant-evaluation context: avoid runtime-only ops.
        return n * 2;
    } else {
        return n * 3;
    }
}

int main() {
    demo::title("C++23 if consteval");
    static_assert(magic(5) == 10);     // constant-evaluated -> 2*5
    int x = 5;
    int y = magic(x);                  // runtime          -> 3*5
    DEMO_ASSERT(y == 15);
    return 0;
}
