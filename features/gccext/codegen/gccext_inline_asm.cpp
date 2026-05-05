// gcc-test: std=c++17 min-gcc=13 topic=gcc-codegen experimental=true
// description: Basic GCC extended inline asm. Demonstrates the input/output operand syntax. x86_64-only; tagged experimental for portability.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html

#include <cassert>

#if defined(__x86_64__) || defined(__i386__)
static inline unsigned long rdtsc_ish() {
    unsigned int lo, hi;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((unsigned long)hi << 32) | lo;
}
#else
static inline unsigned long rdtsc_ish() { return 0; }
#endif

int main() {
#if defined(__x86_64__) || defined(__i386__)
    auto t1 = rdtsc_ish();
    for (volatile int i = 0; i < 1000; ++i) {}
    auto t2 = rdtsc_ish();
    assert(t2 >= t1);   // monotonic on the same core
#else
    (void)rdtsc_ish();
#endif
    return 0;
}
