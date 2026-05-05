// gcc-test: std=c++23 min-gcc=14 topic=gcc-release experimental=false
// description: GCC 14 was the first release where libstdc++ shipped std::ranges::to in the main library (no extra flags).
// reference: https://gcc.gnu.org/gcc-14/changes.html

#include <cassert>
#include <ranges>
#include <vector>

int main() {
    auto v = std::views::iota(0, 5)
           | std::ranges::to<std::vector<int>>();
    assert(v.size() == 5);
    assert(v.front() == 0);
    assert(v.back()  == 4);
    return 0;
}
