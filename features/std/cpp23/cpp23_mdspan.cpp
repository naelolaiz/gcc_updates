// gcc-test: std=c++23 min-gcc=16 topic=stl experimental=false
// description: std::mdspan is a non-owning view over multi-dimensional contiguous data; supports static and dynamic extents.
// reference: https://en.cppreference.com/w/cpp/container/mdspan
// note: <mdspan> first appeared in libstdc++ 16; on gcc-15 the header is missing, so min-gcc=16.

#include "support/demo.hpp"
#include <array>
#include <cassert>
#include <mdspan>
#include <vector>

int main() {
    demo::title("C++23 mdspan");
    // Backing storage: 12 ints arranged as a 3x4 row-major matrix.
    std::array<int, 12> buf{
        1, 2, 3, 4,
        5, 6, 7, 8,
        9,10,11,12,
    };

    // Fully-static extents: rows=3, cols=4.
    std::mdspan<int, std::extents<size_t, 3, 4>> m(buf.data());
    // DEMO_ASSERT() is macro-style; multi-arg subscripts need an extra paren.
    DEMO_ASSERT((m[0, 0] == 1));
    DEMO_ASSERT((m[1, 2] == 7));
    DEMO_ASSERT((m[2, 3] == 12));
    DEMO_ASSERT(m.extent(0) == 3);
    DEMO_ASSERT(m.extent(1) == 4);

    // Dynamic extents -- shape decided at runtime.
    std::mdspan<int, std::dextents<size_t, 2>> m2(buf.data(), 4, 3);
    DEMO_ASSERT(m2.extent(0) == 4);
    DEMO_ASSERT(m2.extent(1) == 3);
    int sum = 0;
    for (size_t r = 0; r < m2.extent(0); ++r)
        for (size_t c = 0; c < m2.extent(1); ++c)
            sum += m2[r, c];   // not inside DEMO_ASSERT(), comma is fine here
    DEMO_ASSERT(sum == 78);   // 1+2+...+12
    return 0;
}
