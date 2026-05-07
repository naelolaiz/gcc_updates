// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: views::enumerate yields (index, value) pairs -- the standard equivalent of Python's enumerate().
// reference: https://en.cppreference.com/w/cpp/ranges/enumerate_view

#include <cassert>
#include <ranges>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> names{"alice", "bob", "carol"};
    int sum_of_indices = 0;
    std::string concatenated;
    for (auto [i, name] : names | std::views::enumerate) {
        sum_of_indices += static_cast<int>(i);
        concatenated += name;
    }
    assert(sum_of_indices == 0 + 1 + 2);
    assert(concatenated == "alicebobcarol");
    return 0;
}
