// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::ssize returns a SIGNED size, eliminating the int/size_t mixed-comparison warnings of c.size().
// reference: https://en.cppreference.com/w/cpp/iterator/size

#include <array>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4};
    auto n = std::ssize(v);
    static_assert(std::is_signed_v<decltype(n)>);
    assert(n == 4);

    int arr[7]{};
    auto m = std::ssize(arr);
    static_assert(std::is_signed_v<decltype(m)>);
    assert(m == 7);

    // Compare cleanly against signed int without -Wsign-compare warnings.
    int target = 4;
    assert(target == std::ssize(v));
    return 0;
}
