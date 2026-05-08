// description: __attribute__((no_sanitize("undefined"))) opts a single function out of UBSan -- useful when a hot path provably can't trip the check.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include "support/demo.hpp"
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
    demo::title("GCC extension no sanitize attribute");
    // The example compiles cleanly with sanitizers AND without; the attribute
    // only changes whether sanitizer instrumentation is inserted.
    DEMO_ASSERT(fast_unsigned_overflow(1, 2) == 3);
    DEMO_ASSERT(never_traps(7) == 7);
    return 0;
}
