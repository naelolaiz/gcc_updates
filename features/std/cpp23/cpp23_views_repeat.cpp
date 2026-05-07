// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: views::repeat yields the same value forever (or N times); composes with views::take for finite ranges.
// reference: https://en.cppreference.com/w/cpp/ranges/repeat_view

#include <cassert>
#include <ranges>
#include <vector>

int main() {
    // Bounded form: 4 copies of 'X'.
    auto bounded = std::views::repeat('X', 4) | std::ranges::to<std::vector<char>>();
    assert((bounded == std::vector<char>{'X', 'X', 'X', 'X'}));

    // Unbounded form -- compose with take.
    auto first_three_zeros = std::views::repeat(0)
                             | std::views::take(3)
                             | std::ranges::to<std::vector<int>>();
    assert((first_three_zeros == std::vector<int>{0, 0, 0}));
    return 0;
}
