// description: std::ranges::to materialises a view (or any range) into a concrete container in one expression.
// reference: https://en.cppreference.com/w/cpp/ranges/to
// why: A lazy pipeline often needs one clear materialization point into owned storage.
// before: Callers spelled iterator-pair constructors or copy loops at the end of a pipeline.
// pitfall: Materialization allocates and applies the destination container's duplicate/order rules.

#include "support/demo.hpp"
#include <cassert>
#include <list>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 ranges to");
    auto squares = std::views::iota(1, 6)
                 | std::views::transform([](int n) { return n * n; })
                 | std::ranges::to<std::vector<int>>();
    DEMO_ASSERT((squares == std::vector<int>{1, 4, 9, 16, 25}));

    // Pipe form.
    auto evens = std::views::iota(0, 10)
               | std::views::filter([](int n) { return n % 2 == 0; })
               | std::ranges::to<std::list<int>>();
    DEMO_ASSERT(evens.size() == 5);
    DEMO_ASSERT(evens.front() == 0);
    DEMO_ASSERT(evens.back() == 8);

    // Across container kinds: list -> vector.
    std::list<int> source{3, 1, 4, 1, 5, 9, 2, 6};
    auto v = source | std::ranges::to<std::vector<int>>();
    DEMO_ASSERT(v.size() == 8);

    // Into an associative container -- de-duplicates.
    auto s = std::vector<int>{1,2,2,3,3,3,4} | std::ranges::to<std::set<int>>();
    DEMO_ASSERT(s == std::set<int>({1,2,3,4}));
    return 0;
}
