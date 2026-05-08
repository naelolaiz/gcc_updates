// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=undefined expect-exit=1
// description: UBSan demo: shifting an int by a count >= the type's width is UB. Runs only under -fsanitize=undefined.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include "support/demo.hpp"

[[gnu::noinline]] int shift_left(int v, int n) { return v << n; }   // UB if n >= 32


int main() {
    demo::title("GCC extension ubsan shift");
    volatile int n = 33;
    return shift_left(1, n);   // UBSan: shift exponent 33 too large for 32-bit type
}
