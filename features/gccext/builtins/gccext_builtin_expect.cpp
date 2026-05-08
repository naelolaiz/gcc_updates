// description: __builtin_expect(expr, val) is the GCC predecessor of [[likely]]/[[unlikely]]; still useful in C and pre-C++20.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html

#include "support/demo.hpp"
#include <cassert>

#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

int classify(int x) {
    if (UNLIKELY(x < 0)) return -1;
    if (LIKELY(x > 0))   return 1;
    return 0;
}

int main() {
    demo::title("GCC extension builtin expect");
    DEMO_ASSERT(classify(-5) == -1);
    DEMO_ASSERT(classify(0) == 0);
    DEMO_ASSERT(classify(7) == 1);

    // __builtin_expect_with_probability lets you specify the probability (GCC 9+).
    int v = 3;
    int r = __builtin_expect_with_probability(v > 0, 1, 0.99);
    DEMO_ASSERT(r == 1);
    return 0;
}
