// description: views::chunk_by splits a range into subranges at every adjacent pair where the predicate turns false -- group ascending runs or equal neighbours without hand-written loops.
// reference: https://en.cppreference.com/w/cpp/ranges/chunk_by_view

#include "support/demo.hpp"
#include <functional>
#include <ranges>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 views::chunk_by");

    // Split into ascending runs: a new chunk starts wherever a > b.
    std::vector<int> v{1, 2, 4, 3, 5, 2, 8, 9};
    std::vector<std::vector<int>> runs;
    for (auto chunk : v | std::views::chunk_by(std::ranges::less_equal{})) {
        runs.emplace_back(chunk.begin(), chunk.end());
    }
    DEMO_ASSERT(runs.size() == 3);
    DEMO_ASSERT((runs[0] == std::vector<int>{1, 2, 4}));
    for (const auto& r : runs) demo::range("run", r);

    // Group equal neighbours (Haskell's 'group'): unlike views::chunk (fixed
    // size N), chunk_by decides the boundaries by looking at the data.
    std::string s = "aabbbcd";
    int groups = 0;
    for (auto g : s | std::views::chunk_by(std::ranges::equal_to{})) {
        (void)g;
        ++groups;
    }
    DEMO_ASSERT(groups == 4);
    return 0;
}
