// gcc-test: std=c++23 min-gcc=14 topic=language experimental=false
// description: 'if consteval' is the standardised, simpler replacement for std::is_constant_evaluated().
// reference: https://en.cppreference.com/w/cpp/language/if#Consteval_if

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
    static_assert(magic(5) == 10);     // constant-evaluated -> 2*5
    int x = 5;
    int y = magic(x);                  // runtime          -> 3*5
    assert(y == 15);
    return 0;
}
