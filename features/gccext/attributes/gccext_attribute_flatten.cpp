// gcc-test: std=c++17 min-gcc=13 topic=gcc-attributes experimental=false
// description: [[gnu::flatten]] inlines every direct call inside the marked function -- careful: bloats code.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include <cassert>

inline int mul_by_3(int x) { return x * 3; }
inline int mul_by_5(int x) { return x * 5; }

[[gnu::flatten]]
int fused(int x) {
    // Both inline calls below are forced inline at this call site.
    return mul_by_3(x) + mul_by_5(x);
}

int main() {
    assert(fused(2) == 6 + 10);
    assert(fused(10) == 30 + 50);
    return 0;
}
