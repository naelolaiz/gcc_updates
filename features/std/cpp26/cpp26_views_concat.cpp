// description: std::views::concat presents several ranges as one sequence without copying them, and stays writable when every input is.
// reference: https://en.cppreference.com/w/cpp/ranges/concat_view
// why: Code can iterate, search, or modify the logical union of separate buffers as one range.
// before: Copy everything into a temporary container, or write nested loops over each piece.
// pitfall: The element type is the common reference of all inputs, so mixing value types can force copies instead of references.

#include "support/demo.hpp"
#include <algorithm>
#include <array>
#include <list>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++26 views::concat");

    std::vector<int> head{1, 2};
    std::array<int, 2> middle{3, 4};
    std::list<int> tail{5};

    auto all = std::views::concat(head, middle, tail);
    demo::range("concat(vector, array, list)", all);
    DEMO_ASSERT(std::ranges::distance(all) == 5);
    DEMO_ASSERT(std::ranges::equal(all, std::vector{1, 2, 3, 4, 5}));

    // Writes go through to the underlying containers.
    for (int& x : all) x *= 10;
    DEMO_ASSERT(head.front() == 10 && middle.back() == 40 && tail.front() == 50);

    // Composes with other views like any range.
    auto evens = all | std::views::filter([](int x) { return x % 20 == 0; });
    DEMO_ASSERT(std::ranges::equal(evens, std::vector{20, 40}));
    return 0;
}
