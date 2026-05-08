// description: views::cartesian_product yields the Cartesian product of N input ranges as tuples.
// reference: https://en.cppreference.com/w/cpp/ranges/cartesian_product_view

#include "support/demo.hpp"
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++23 ranges cartesian product");
    std::vector<int>  a{1, 2, 3};
    std::vector<char> b{'x', 'y'};

    int count = 0;
    int sum = 0;
    for (auto [n, c] : std::views::cartesian_product(a, b)) {
        ++count;
        sum += n + (c - 'x');
    }
    // 3 * 2 = 6 tuples
    DEMO_ASSERT(count == 6);
    // Each n appears 2x (once with x=0, once with y=1) -> 2*(1+2+3) = 12; plus
    // one '+1' per pair where c=='y' -> +3. Total = 15.
    DEMO_ASSERT(sum == 15);

    // 3-way cartesian product compiles too.
    int triples = 0;
    for ([[maybe_unused]] auto t :
         std::views::cartesian_product(a, b, std::vector{0, 0, 0, 0})) {
        ++triples;
    }
    DEMO_ASSERT(triples == 3 * 2 * 4);
    return 0;
}
