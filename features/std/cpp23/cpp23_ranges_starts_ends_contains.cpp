// gcc-test: std=c++23 min-gcc=14 min-libstdcxx=16 topic=ranges experimental=false
// description: ranges::starts_with / ends_with / contains / contains_subrange -- algorithm form of the string-shaped predicates.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges/starts_with
// note: per cppreference, ranges::contains/contains_subrange shipped in libstdc++ 13,
// but ranges::starts_with/ends_with require libstdc++ 16+. Since this single file
// exercises all four, gate the whole thing on libstdc++ 16+. Symbols live in
// <algorithm>, not <ranges>.

#include "support/demo.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++23 ranges starts ends contains");
    std::vector<int> v{1, 2, 3, 4, 5};

    DEMO_ASSERT(std::ranges::starts_with(v, std::vector<int>{1, 2}));
    DEMO_ASSERT(!std::ranges::starts_with(v, std::vector<int>{2, 3}));

    DEMO_ASSERT(std::ranges::ends_with(v, std::vector<int>{4, 5}));
    DEMO_ASSERT(!std::ranges::ends_with(v, std::vector<int>{1, 2}));

    DEMO_ASSERT(std::ranges::contains(v, 3));
    DEMO_ASSERT(!std::ranges::contains(v, 99));

    DEMO_ASSERT(std::ranges::contains_subrange(v, std::vector<int>{2, 3, 4}));
    DEMO_ASSERT(!std::ranges::contains_subrange(v, std::vector<int>{3, 5}));
    return 0;
}
