// gcc-test: std=c++17 min-gcc=13 topic=gcc-attributes experimental=true
// description: [[gnu::target("avx2")]] compiles ONE function with extra ISA flags; the rest of the TU stays at the default ISA.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
// note: experimental because the runner CPU must support AVX2; if it doesn't, the program SIGILLs.

#include <cassert>

[[gnu::target("avx2")]]
int dot_avx(const int* a, const int* b, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += a[i] * b[i];   // GCC may auto-vectorize with AVX2 / 256-bit ymm
    return s;
}

int dot_default(const int* a, const int* b, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += a[i] * b[i];
    return s;
}

int main() {
    int a[8]{1, 2, 3, 4, 5, 6, 7, 8};
    int b[8]{8, 7, 6, 5, 4, 3, 2, 1};
    int expected = 1*8 + 2*7 + 3*6 + 4*5 + 5*4 + 6*3 + 7*2 + 8*1;
    assert(dot_default(a, b, 8) == expected);
    assert(dot_avx(a, b, 8) == expected);
    return 0;
}
