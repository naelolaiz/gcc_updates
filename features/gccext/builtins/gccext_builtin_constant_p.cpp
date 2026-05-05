// gcc-test: std=c++17 min-gcc=13 topic=gcc-builtins experimental=false
// description: __builtin_constant_p(x) is true at compile time iff x is a constant expression. Used to pick a faster path for known inputs.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html

#include <cassert>

constexpr int slow_path(int x) { return x * x + 1; }

inline int specialised(int x) {
    if (__builtin_constant_p(x)) {
        // Compile-time path: cheaper code, fully evaluated at compile time.
        return x * x + 1;
    }
    return slow_path(x);
}

int main() {
    // Constant input -> compile-time evaluation.
    static_assert(__builtin_constant_p(5), "");
    assert(specialised(5) == 26);

    // Runtime input -> runtime path.
    int v = 6;
    assert(specialised(v) == 37);
    return 0;
}
