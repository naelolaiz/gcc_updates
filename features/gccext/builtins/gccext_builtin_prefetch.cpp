// description: __builtin_prefetch(addr, rw, locality) asks the CPU to pull a cache line early; purely a hint -- a wrong guess costs memory bandwidth, never correctness.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html

#include "support/demo.hpp"
#include <cstddef>
#include <numeric>
#include <vector>

// Sum with a software prefetch a few iterations ahead. At this toy size the
// hardware prefetcher wins anyway -- the point is the calling convention:
// rw 0=read 1=write, locality 0 (no reuse) .. 3 (keep in all cache levels).
long sum_with_prefetch(const std::vector<long>& v) {
    long total = 0;
    constexpr std::size_t ahead = 16;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i + ahead < v.size()) {
            __builtin_prefetch(&v[i + ahead], 0, 1);
        }
        total += v[i];
    }
    return total;
}

int main() {
    demo::title("GCC __builtin_prefetch");

    std::vector<long> v(1 << 16);
    std::iota(v.begin(), v.end(), 0L);

    const long n = static_cast<long>(v.size());
    const long expected = n * (n - 1) / 2;
    DEMO_ASSERT(sum_with_prefetch(v) == expected);
    demo::value("sum", expected);
    return 0;
}
