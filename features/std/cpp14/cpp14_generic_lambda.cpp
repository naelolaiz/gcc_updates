// description: Generic lambdas: 'auto' parameters make the lambda's call operator a template -- one lambda works across ints, strings, and any pair type without spelling the types.
// reference: https://en.cppreference.com/w/cpp/language/lambda

#include "support/demo.hpp"
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

int main() {
    demo::title("C++14 generic lambda");

    // 'auto' in the parameter list turns operator() into a template.
    auto twice = [](auto x) { return x + x; };
    DEMO_ASSERT(twice(21) == 42);
    DEMO_ASSERT(twice(std::string("ab")) == "abab");
    DEMO_ASSERT(twice(1.5) == 3.0);

    // The classic win: one comparator without spelling
    // std::pair<std::string, int> in full.
    std::vector<std::pair<std::string, int>> scores{
        {"carol", 3}, {"alice", 9}, {"bob", 5}};
    std::sort(scores.begin(), scores.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    DEMO_ASSERT(scores.front().second == 9);
    demo::range("by score desc", scores);

    // Works with algorithms that mix accumulator and element types.
    std::vector<int> v{1, 2, 3, 4};
    auto product = std::accumulate(v.begin(), v.end(), 1L,
                                   [](auto acc, auto x) { return acc * x; });
    DEMO_ASSERT(product == 24);

    // Variadic 'auto...' packs work too.
    auto count_args = [](auto... xs) { return sizeof...(xs); };
    DEMO_ASSERT(count_args(1, 'a', 2.0) == 3);
    return 0;
}
