// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: std::array<T,N> is a fixed-size, value-semantic, container-like wrapper around a C array; size in the type, no decay to pointer.
// reference: https://en.cppreference.com/w/cpp/container/array

#include "support/demo.hpp"
#include <array>
#include <cassert>
#include <numeric>

int main() {
    demo::title("C++11 array");
    std::array<int, 5> a{1, 2, 3, 4, 5};

    DEMO_ASSERT(a.size() == 5);
    DEMO_ASSERT(a.front() == 1);
    DEMO_ASSERT(a.back() == 5);
    DEMO_ASSERT(a[2] == 3);

    int s = std::accumulate(a.begin(), a.end(), 0);
    DEMO_ASSERT(s == 15);

    // Value semantics: copyable, comparable.
    std::array<int, 5> b = a;
    DEMO_ASSERT(b == a);
    b[0] = 99;
    DEMO_ASSERT(b != a);

    // No pointer decay: size is part of the type.
    constexpr std::size_t N = std::tuple_size<decltype(a)>::value;
    static_assert(N == 5, "");
    return 0;
}
