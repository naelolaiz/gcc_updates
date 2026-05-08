// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: views::enumerate yields (index, value) pairs -- the standard equivalent of Python's enumerate().
// reference: https://en.cppreference.com/w/cpp/ranges/enumerate_view

#include "support/demo.hpp"
#include <cassert>
#include <ranges>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 ranges enumerate");
    std::vector<std::string> names{"alice", "bob", "carol"};
    int sum_of_indices = 0;
    std::string concatenated;
    for (auto [i, name] : names | std::views::enumerate) {
        sum_of_indices += static_cast<int>(i);
        concatenated += name;
    }
    DEMO_ASSERT(sum_of_indices == 0 + 1 + 2);
    DEMO_ASSERT(concatenated == "alicebobcarol");
    return 0;
}
