// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false
// description: __attribute__((no_sanitize("undefined"))) opts a single function out of UBSan -- useful when a hot path provably can't trip the check.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include <cassert>
#include <cstdint>

[[gnu::no_sanitize("undefined")]]
unsigned long long fast_unsigned_overflow(unsigned long long a,
                                          unsigned long long b) {
    // Wraparound on unsigned IS defined; this attribute matters more for
    // ('signed-integer-overflow', 'shift-base', etc.) where we want to opt out.
    return a + b;
}

[[gnu::no_sanitize("address", "undefined")]]
int never_traps(int x) { return x; }

int main() {
    // The example compiles cleanly with sanitizers AND without; the attribute
    // only changes whether sanitizer instrumentation is inserted.
    assert(fast_unsigned_overflow(1, 2) == 3);
    assert(never_traps(7) == 7);
    return 0;
}
