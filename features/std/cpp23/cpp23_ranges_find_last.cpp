// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=true
// description: ranges::find_last / find_last_if / find_last_if_not return the LAST matching position; previously you reversed first.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges/find_last

#include <cassert>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 3, 2, 1};

    // find_last returns a subrange starting at the last match.
    auto sub = std::ranges::find_last(v, 3);
    assert(!sub.empty());
    assert(sub.front() == 3);
    assert(std::distance(v.begin(), sub.begin()) == 4);   // index of the LAST 3

    auto last_even = std::ranges::find_last_if(v, [](int x) { return x % 2 == 0; });
    assert(!last_even.empty());
    assert(last_even.front() == 2);
    assert(std::distance(v.begin(), last_even.begin()) == 5);

    auto missing = std::ranges::find_last(v, 99);
    assert(missing.empty());
    return 0;
}
