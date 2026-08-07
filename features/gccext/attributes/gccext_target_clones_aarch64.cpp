// description: [[gnu::target_clones("default", "sve")]] works on AArch64 from GCC 14 (ACLE function multi-versioning): an IFUNC picks the SVE clone when the CPU has it, the default clone otherwise.
// reference: https://gcc.gnu.org/onlinedocs/gcc/AArch64-Function-Attributes.html
// note: AArch64 FMV is still maturing: GCC 15 warns -Wexperimental-fmv-target, silenced in CMakeLists.txt (GCC 14 ignores the unknown -Wno- option).

#include "support/demo.hpp"
#include <cassert>

// AArch64 twin of gccext_target_clones (x86 avx2): same automatic IFUNC
// dispatch, with an SVE clone instead. Safe on non-SVE cores (e.g. Apple
// Silicon): the resolver falls back to the 'default' clone at load time.
[[gnu::target_clones("default", "sve")]]
int sum(const int* p, int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += p[i];
    return s;
}

int main() {
    demo::title("GCC extension target clones (AArch64)");
    int v[16];
    for (int i = 0; i < 16; ++i) v[i] = i + 1;     // 1..16
    DEMO_ASSERT(sum(v, 16) == 136);                      // 16*17/2
    return 0;
}
