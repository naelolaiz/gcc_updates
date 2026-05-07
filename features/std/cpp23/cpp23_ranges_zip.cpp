// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: std::views::zip turns parallel ranges into a single range of tuples; stops at shortest input.
// reference: https://en.cppreference.com/w/cpp/ranges/zip_view

#include <cassert>
#include <ranges>
#include <string>
#include <tuple>
#include <vector>

int main() {
    std::vector<int>         ids   {1, 2, 3, 4};
    std::vector<std::string> names {"alice", "bob", "carol"};
    std::vector<double>      ratios{0.1, 0.2, 0.3, 0.4, 0.5};

    int count = 0;
    for (auto [id, name, ratio] : std::views::zip(ids, names, ratios)) {
        (void)id; (void)name; (void)ratio;
        ++count;
    }
    // Stops at shortest = names.size() = 3.
    assert(count == 3);

    // zip_transform: zip + transform fused.
    auto sums = std::views::zip_transform(std::plus<>{},
                                          std::vector{1, 2, 3},
                                          std::vector{10, 20, 30});
    int total = 0;
    for (int v : sums) total += v;
    assert(total == 66);   // 11 + 22 + 33
    return 0;
}
