// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: std::tuple holds a heterogeneous fixed-size pack; std::tie + std::ignore destructure (C++17 added structured bindings).
// reference: https://en.cppreference.com/w/cpp/utility/tuple

#include <cassert>
#include <string>
#include <tuple>

std::tuple<int, std::string, double> three() { return std::make_tuple(1, "two", 3.0); }

int main() {
    auto t = three();
    assert(std::get<0>(t) == 1);
    assert(std::get<1>(t) == "two");
    assert(std::get<2>(t) == 3.0);

    // C++14 added get<T> (by type); use get<index> in pure C++11.

    // Destructure with std::tie.
    int a; std::string b; double c;
    std::tie(a, b, c) = three();
    assert(a == 1 && b == "two" && c == 3.0);

    // std::ignore drops a slot.
    int x;
    std::tie(x, std::ignore, std::ignore) = three();
    assert(x == 1);

    // Comparison is element-wise lexicographic.
    auto t1 = std::make_tuple(1, 2);
    auto t2 = std::make_tuple(1, 3);
    assert(t1 < t2);
    return 0;
}
