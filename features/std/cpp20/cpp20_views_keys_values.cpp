// description: views::keys / views::values / views::elements project tuple-like ranges to a single component.
// reference: https://en.cppreference.com/w/cpp/ranges/elements_view

#include "support/demo.hpp"
#include <cassert>
#include <map>
#include <ranges>
#include <string>
#include <vector>

int main() {
    demo::title("C++20 views keys values");
    std::map<std::string, int> ages{{"alice", 30}, {"bob", 25}, {"carol", 40}};

    // views::keys yields the first element of each pair.
    int total_keys_len = 0;
    for (const auto& k : ages | std::views::keys) {
        total_keys_len += static_cast<int>(k.size());
    }
    DEMO_ASSERT(total_keys_len == 5 + 3 + 5);

    // views::values yields the second.
    int sum_ages = 0;
    for (int v : ages | std::views::values) sum_ages += v;
    DEMO_ASSERT(sum_ages == 95);

    // views::elements<N> for arbitrary tuple-likes.
    std::vector<std::tuple<int, std::string, double>> rows{
        {1, "a", 1.5}, {2, "b", 2.5}, {3, "c", 3.5}};
    double sum_doubles = 0;
    for (double d : rows | std::views::elements<2>) sum_doubles += d;
    DEMO_ASSERT(sum_doubles == 7.5);
    return 0;
}
