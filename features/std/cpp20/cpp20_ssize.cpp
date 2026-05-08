// description: std::ssize returns a SIGNED size, eliminating the int/size_t mixed-comparison warnings of c.size().
// reference: https://en.cppreference.com/w/cpp/iterator/size

#include "support/demo.hpp"
#include <array>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <vector>

int main() {
    demo::title("C++20 ssize");
    std::vector<int> v{1, 2, 3, 4};
    auto n = std::ssize(v);
    static_assert(std::is_signed_v<decltype(n)>);
    DEMO_ASSERT(n == 4);

    int arr[7]{};
    auto m = std::ssize(arr);
    static_assert(std::is_signed_v<decltype(m)>);
    DEMO_ASSERT(m == 7);

    // Compare cleanly against signed int without -Wsign-compare warnings.
    int target = 4;
    DEMO_ASSERT(target == std::ssize(v));
    return 0;
}
