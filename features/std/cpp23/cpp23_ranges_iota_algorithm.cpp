// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: std::ranges::iota (the algorithm, not the view) fills a range with sequentially increasing values; concept-checked.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges/iota

#include <cassert>
#include <numeric>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v(5);
    // Algorithm form fills v with 10, 11, 12, 13, 14.
    std::ranges::iota(v, 10);
    assert((v == std::vector<int>{10, 11, 12, 13, 14}));

    // The view form (since C++20) is a lazy bounded sequence; not the same thing.
    auto lazy = std::views::iota(0, 4) | std::ranges::to<std::vector<int>>();
    assert((lazy == std::vector<int>{0, 1, 2, 3}));
    return 0;
}
