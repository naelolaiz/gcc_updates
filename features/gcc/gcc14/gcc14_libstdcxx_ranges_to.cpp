// description: GCC 14 was the first release where libstdc++ shipped std::ranges::to in the main library (no extra flags).
// reference: https://gcc.gnu.org/gcc-14/changes.html

#include "support/demo.hpp"
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    demo::title("GCC 14 libstdcxx ranges to");
    auto v = std::views::iota(0, 5)
           | std::ranges::to<std::vector<int>>();
    DEMO_ASSERT(v.size() == 5);
    DEMO_ASSERT(v.front() == 0);
    DEMO_ASSERT(v.back()  == 4);
    return 0;
}
