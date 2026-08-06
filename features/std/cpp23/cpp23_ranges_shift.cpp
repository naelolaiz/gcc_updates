// description: std::ranges::shift_left and shift_right move elements within a range and return the subrange containing the useful result.
// reference: https://gcc.gnu.org/gcc-16/changes.html
// why: The returned subrange makes the still-valid portion explicit after shifting elements.
// before: The iterator algorithm required callers to reconstruct the useful range themselves.
// pitfall: Elements outside the returned subrange are valid but have unspecified values.

#include "support/demo.hpp"
#include <algorithm>
#include <vector>

int main() {
    demo::title("C++23 ranges shift");
    std::vector<int> values{1, 2, 3, 4, 5};
    auto result = std::ranges::shift_left(values, 2);
    DEMO_ASSERT(result.size() == 3);
    DEMO_ASSERT((std::vector<int>(result.begin(), result.end()) ==
                 std::vector<int>{3, 4, 5}));
    return 0;
}
