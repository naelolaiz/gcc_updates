// description: __builtin_assume_aligned(p, N) tells the optimizer 'p is N-byte aligned' -- enables aligned vector loads.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html

#include "support/demo.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>

int sum_aligned(const int* in, std::size_t n) {
    auto* p = static_cast<const int*>(__builtin_assume_aligned(in, 32));
    int s = 0;
    for (std::size_t i = 0; i < n; ++i) s += p[i];
    return s;
}

int main() {
    demo::title("GCC extension builtin assume aligned");
    alignas(32) int buf[8]{1, 2, 3, 4, 5, 6, 7, 8};
    DEMO_ASSERT(reinterpret_cast<std::uintptr_t>(buf) % 32 == 0);
    DEMO_ASSERT(sum_aligned(buf, 8) == 36);
    return 0;
}
