// description: [[gnu::pure]] = no side effects (may read globals); [[gnu::const]] = depends only on args. Both let the compiler eliminate redundant calls.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include "support/demo.hpp"
#include <cassert>

[[gnu::const]]
int square(int x) { return x * x; }

[[gnu::pure]]
int strlen_like(const char* s) {
    int n = 0;
    while (*s++) ++n;
    return n;
}

int main() {
    demo::title("GCC extension attribute pure const");
    DEMO_ASSERT(square(5) == 25);

    // The compiler can hoist/dedupe pure-and-const calls.
    int a = square(7) + square(7);   // may compute square(7) once
    DEMO_ASSERT(a == 98);

    DEMO_ASSERT(strlen_like("hello") == 5);
    return 0;
}
