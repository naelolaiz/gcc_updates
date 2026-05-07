// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=true
// description: ranges::starts_with / ends_with / contains / contains_subrange -- algorithm form of the string-shaped predicates.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges/starts_with

#include <cassert>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    assert(std::ranges::starts_with(v, std::vector<int>{1, 2}));
    assert(!std::ranges::starts_with(v, std::vector<int>{2, 3}));

    assert(std::ranges::ends_with(v, std::vector<int>{4, 5}));
    assert(!std::ranges::ends_with(v, std::vector<int>{1, 2}));

    assert(std::ranges::contains(v, 3));
    assert(!std::ranges::contains(v, 99));

    assert(std::ranges::contains_subrange(v, std::vector<int>{2, 3, 4}));
    assert(!std::ranges::contains_subrange(v, std::vector<int>{3, 5}));
    return 0;
}
