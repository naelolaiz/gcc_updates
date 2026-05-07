// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: std::tuple holds a heterogeneous fixed-size pack; std::tie + std::ignore destructure (C++17 added structured bindings).
// reference: https://en.cppreference.com/w/cpp/utility/tuple

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <tuple>

std::tuple<int, std::string, double> three() { return std::make_tuple(1, "two", 3.0); }

int main() {
    demo::title("C++11 tuple");
    auto t = three();
    DEMO_ASSERT(std::get<0>(t) == 1);
    DEMO_ASSERT(std::get<1>(t) == "two");
    DEMO_ASSERT(std::get<2>(t) == 3.0);

    // C++14 added get<T> (by type); use get<index> in pure C++11.

    // Destructure with std::tie.
    int a; std::string b; double c;
    std::tie(a, b, c) = three();
    DEMO_ASSERT(a == 1 && b == "two" && c == 3.0);

    // std::ignore drops a slot.
    int x;
    std::tie(x, std::ignore, std::ignore) = three();
    DEMO_ASSERT(x == 1);

    // Comparison is element-wise lexicographic.
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::make_tuple(1, 3);
    DEMO_ASSERT(t1 < t2);
    return 0;
}
