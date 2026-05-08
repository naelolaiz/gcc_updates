// description: views::adjacent<N> yields N-tuples of consecutive elements; pairwise = adjacent<2>.
// reference: https://en.cppreference.com/w/cpp/ranges/adjacent_view

#include "support/demo.hpp"
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++23 ranges adjacent");
    std::vector<int> v{10, 20, 30, 40, 50};

    // Pairwise positive deltas using views::adjacent_transform. The lambda
    // takes (current, next) so we compute b - a to get +10 each time.
    auto diffs = v | std::views::adjacent_transform<2>(
                          [](int a, int b) { return b - a; });
    std::vector<int> got;
    for (int d : diffs) got.push_back(d);
    DEMO_ASSERT((got == std::vector<int>{10, 10, 10, 10}));

    // adjacent<3>: triples of consecutive elements.
    int triple_count = 0;
    for (auto [a, b, c] : v | std::views::adjacent<3>) {
        (void)a; (void)b; (void)c;
        ++triple_count;
    }
    DEMO_ASSERT(triple_count == 3);   // (10,20,30) (20,30,40) (30,40,50)
    return 0;
}
