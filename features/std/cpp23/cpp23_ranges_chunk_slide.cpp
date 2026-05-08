// description: views::chunk splits a range into non-overlapping windows of size N; views::slide does overlapping windows.
// reference: https://en.cppreference.com/w/cpp/ranges/chunk_view

#include "support/demo.hpp"
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    demo::title("C++23 ranges chunk slide");
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7};

    std::vector<std::vector<int>> chunks;
    for (auto sub : v | std::views::chunk(3)) {
        chunks.emplace_back(sub.begin(), sub.end());
    }
    DEMO_ASSERT(chunks.size() == 3);
    DEMO_ASSERT((chunks[0] == std::vector<int>{1, 2, 3}));
    DEMO_ASSERT((chunks[1] == std::vector<int>{4, 5, 6}));
    DEMO_ASSERT((chunks[2] == std::vector<int>{7}));

    std::vector<std::vector<int>> slides;
    for (auto sub : v | std::views::slide(3)) {
        slides.emplace_back(sub.begin(), sub.end());
    }
    // 7 elements with window size 3 => 5 windows.
    DEMO_ASSERT(slides.size() == 5);
    DEMO_ASSERT((slides[0] == std::vector<int>{1, 2, 3}));
    DEMO_ASSERT((slides[4] == std::vector<int>{5, 6, 7}));
    return 0;
}
