// description: std::submdspan creates a lower-dimensional or sliced mdspan while preserving the mapping and accessor needed to view the original storage.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2630r4.html
// why: Matrix tiles and rows can be passed without copying or manually computing offsets.
// before: Pointer arithmetic paired with separately tracked shape and stride values.
// pitfall: The slice is non-owning and cannot outlive the backing storage.

#include "support/demo.hpp"
#include <array>
#include <mdspan>
#include <utility>

int main() {
    demo::title("C++26 submdspan");
    std::array<int, 12> storage{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::mdspan matrix(storage.data(), 3, 4);
    auto middle_columns = std::submdspan(
        matrix, std::full_extent, std::pair{1, 3});

    DEMO_ASSERT(middle_columns.extent(0) == 3);
    DEMO_ASSERT(middle_columns.extent(1) == 2);
    DEMO_ASSERT((middle_columns[0, 0] == 2));
    DEMO_ASSERT((middle_columns[2, 1] == 11));
    return 0;
}
