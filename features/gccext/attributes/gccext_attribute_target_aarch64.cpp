// description: [[gnu::target("arch=armv8-a+crc")]] is the AArch64 spelling of per-function ISA selection: ONE function gets extra architecture extensions while the rest of the TU stays at the default ISA.
// reference: https://gcc.gnu.org/onlinedocs/gcc/AArch64-Function-Attributes.html

#include "support/demo.hpp"
#include <cassert>

// AArch64 twin of gccext_attribute_target (x86 avx2). The +crc extension is
// only ENABLED for codegen here, not executed unguarded, so the demo cannot
// SIGILL on older cores the way the executed-avx2 x86 demo can.
[[gnu::target("arch=armv8-a+crc")]]
int dot_ext(const int* a, const int* b, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += a[i] * b[i];   // may auto-vectorize with NEON
    return s;
}

int dot_default(const int* a, const int* b, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += a[i] * b[i];
    return s;
}

int main() {
    demo::title("GCC extension attribute target (AArch64)");
    int a[8]{1, 2, 3, 4, 5, 6, 7, 8};
    int b[8]{8, 7, 6, 5, 4, 3, 2, 1};
    int expected = 1*8 + 2*7 + 3*6 + 4*5 + 5*4 + 6*3 + 7*2 + 8*1;
    DEMO_ASSERT(dot_default(a, b, 8) == expected);
    DEMO_ASSERT(dot_ext(a, b, 8) == expected);
    return 0;
}
