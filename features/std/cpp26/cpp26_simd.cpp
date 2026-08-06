// description: std::simd::vec expresses element-wise arithmetic over a fixed number of lanes and lets the implementation select vector instructions.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p1928r15.pdf
// why: Portable data-parallel code no longer needs compiler vector types or ISA intrinsics.
// before: GCC vector_size types or one intrinsic implementation per target ISA.
// pitfall: Lane count and ABI affect portability; code should not assume a native width.

#include "support/demo.hpp"
#include <simd>

int main() {
    demo::title("C++26 simd");
    using four_ints = std::simd::vec<int, 4>;
    four_ints values([](auto index) { return static_cast<int>(index) + 1; });
    four_ints doubled = values * four_ints(2);

    static_assert(four_ints::size() == 4);
    DEMO_ASSERT(doubled[0] == 2);
    DEMO_ASSERT(doubled[3] == 8);
    DEMO_ASSERT(std::simd::reduce(doubled) == 20);
    return 0;
}
