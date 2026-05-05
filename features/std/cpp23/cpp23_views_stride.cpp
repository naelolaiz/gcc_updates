// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: views::stride takes every N-th element; useful for downsampling.
// reference: https://en.cppreference.com/w/cpp/ranges/stride_view

#include <cassert>
#include <ranges>
#include <vector>

int main() {
    auto evens = std::views::iota(0, 10)
               | std::views::stride(2)
               | std::ranges::to<std::vector<int>>();
    assert((evens == std::vector<int>{0, 2, 4, 6, 8}));

    auto every_third = std::views::iota(0, 10)
                     | std::views::stride(3)
                     | std::ranges::to<std::vector<int>>();
    assert((every_third == std::vector<int>{0, 3, 6, 9}));
    return 0;
}
