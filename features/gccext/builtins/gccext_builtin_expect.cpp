// gcc-test: std=c++17 min-gcc=13 topic=gcc-builtins experimental=false
// description: __builtin_expect(expr, val) is the GCC predecessor of [[likely]]/[[unlikely]]; still useful in C and pre-C++20.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html

#include <cassert>

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

int classify(int x) {
    if (UNLIKELY(x < 0)) return -1;
    if (LIKELY(x > 0))   return 1;
    return 0;
}

int main() {
    assert(classify(-5) == -1);
    assert(classify(0) == 0);
    assert(classify(7) == 1);

    // __builtin_expect_with_probability lets you specify the probability (GCC 9+).
    int v = 3;
    int r = __builtin_expect_with_probability(v > 0, 1, 0.99);
    assert(r == 1);
    return 0;
}
