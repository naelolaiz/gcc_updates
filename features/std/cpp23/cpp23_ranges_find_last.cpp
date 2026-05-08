// description: ranges::find_last / find_last_if / find_last_if_not return the LAST matching position; previously you reversed first.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges/find_last
// note: ranges::find_last* live in <algorithm>, not <ranges>. Per cppreference,
// libstdc++ ships them since release 13.

#include "support/demo.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++23 ranges find last");
    std::vector<int> v{1, 2, 3, 4, 3, 2, 1};

    // find_last returns a subrange starting at the last match.
    auto sub = std::ranges::find_last(v, 3);
    DEMO_ASSERT(!sub.empty());
    DEMO_ASSERT(sub.front() == 3);
    DEMO_ASSERT(std::distance(v.begin(), sub.begin()) == 4);   // index of the LAST 3

    auto last_even = std::ranges::find_last_if(v, [](int x) { return x % 2 == 0; });
    DEMO_ASSERT(!last_even.empty());
    DEMO_ASSERT(last_even.front() == 2);
    DEMO_ASSERT(std::distance(v.begin(), last_even.begin()) == 5);

    auto missing = std::ranges::find_last(v, 99);
    DEMO_ASSERT(missing.empty());
    return 0;
}
