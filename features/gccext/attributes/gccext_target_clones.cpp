// gcc-test: std=c++17 min-gcc=13 topic=gcc-attributes experimental=false skip-sanitizer=thread
// description: [[gnu::target_clones("default,avx2,avx512f")]] generates multiple versions; an IFUNC dispatches to the best at startup.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
// note: experimental: only some target/CPU combinations are supported; binary still runs because of the 'default' fallback.

#include "support/demo.hpp"
#include <cassert>

[[gnu::target_clones("default", "avx2")]]
int sum(const int* p, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += p[i];
    return s;
}

int main() {
    demo::title("GCC extension target clones");
    int v[16];
    for (int i = 0; i < 16; ++i) v[i] = i + 1;     // 1..16
    DEMO_ASSERT(sum(v, 16) == 136);                      // 16*17/2
    return 0;
}
