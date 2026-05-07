// gcc-test: std=c++23 min-gcc=14 min-libstdcxx=13 topic=ranges experimental=false
// description: ranges::fold_left / fold_right / fold_left_first replace std::accumulate; concept-checked, projection-aware.
// reference: https://en.cppreference.com/w/cpp/algorithm/ranges/fold_left
// note: ranges::fold_* live in <algorithm>, not <ranges>. Per cppreference,
// libstdc++ ships them since release 13.

#include "support/demo.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 ranges fold");
    std::vector<int> v{1, 2, 3, 4, 5};

    // fold_left(range, init, op) -- the modern std::accumulate.
    int sum = std::ranges::fold_left(v, 0, std::plus<>{});
    DEMO_ASSERT(sum == 15);

    // fold_left_first uses the first element as the seed (no init).
    auto product = std::ranges::fold_left_first(v, std::multiplies<>{});
    DEMO_ASSERT(product.has_value());
    DEMO_ASSERT(*product == 120);

    // fold_right: right-associative.
    auto concat = std::ranges::fold_right(
        std::vector<std::string>{"a", "b", "c"},
        std::string(""),
        [](const std::string& a, const std::string& b) { return a + b; });
    DEMO_ASSERT(concat == "abc");

    // fold_left_with_iter returns both result and the iterator one-past-last consumed.
    auto [it, total] = std::ranges::fold_left_with_iter(v, 0, std::plus<>{});
    DEMO_ASSERT(total == 15);
    DEMO_ASSERT(it == v.end());
    return 0;
}
