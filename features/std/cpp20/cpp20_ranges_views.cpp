// description: Lazy view composition with views::filter, views::transform and views::take using the | pipe.
// reference: https://en.cppreference.com/w/cpp/ranges
// why: Sequence transformations can be composed without allocating each intermediate result.
// before: Algorithms required iterator pairs and explicit temporary containers between stages.
// pitfall: Views are lazy and often non-owning, so source mutation and lifetime affect later iteration.

#include "support/demo.hpp"
#include <cassert>
#include <numeric>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++20 ranges views");
    std::vector<int> nums(20);
    std::iota(nums.begin(), nums.end(), 1);   // 1..20

    auto pipeline =
        nums
        | std::views::filter([](int n) { return n % 2 == 0; })   // even
        | std::views::transform([](int n) { return n * n; })     // squared
        | std::views::take(3);                                   // first three

    int sum = 0;
    for (int x : pipeline) sum += x;
    // 2*2 + 4*4 + 6*6 = 4 + 16 + 36 = 56
    DEMO_ASSERT(sum == 56);

    return 0;
}
