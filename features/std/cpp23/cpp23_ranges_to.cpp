// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: std::ranges::to materialises a view (or any range) into a concrete container in one expression.
// reference: https://en.cppreference.com/w/cpp/ranges/to

#include <cassert>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <vector>

int main() {
    auto squares = std::views::iota(1, 6)
                 | std::views::transform([](int n) { return n * n; })
                 | std::ranges::to<std::vector<int>>();
    assert((squares == std::vector<int>{1, 4, 9, 16, 25}));

    // Pipe form.
    auto evens = std::views::iota(0, 10)
               | std::views::filter([](int n) { return n % 2 == 0; })
               | std::ranges::to<std::list<int>>();
    assert(evens.size() == 5);
    assert(evens.front() == 0);
    assert(evens.back() == 8);

    // Across container kinds: list -> vector.
    std::list<int> source{3, 1, 4, 1, 5, 9, 2, 6};
    auto v = source | std::ranges::to<std::vector<int>>();
    assert(v.size() == 8);

    // Into an associative container -- de-duplicates.
    auto s = std::vector<int>{1,2,2,3,3,3,4} | std::ranges::to<std::set<int>>();
    assert(s == std::set<int>({1,2,3,4}));
    return 0;
}
