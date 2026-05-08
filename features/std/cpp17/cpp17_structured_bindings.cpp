// gcc-test: std=c++17 min-gcc=13 topic=language experimental=false
// description: Structured bindings: 'auto [a, b, c] = expr;' destructures arrays, tuples/pairs, and aggregates.
// reference: https://en.cppreference.com/w/cpp/language/structured_binding

#include "support/demo.hpp"
#include <array>
#include <cassert>
#include <map>
#include <string>
#include <tuple>

struct Point { int x; int y; };

std::tuple<int, std::string, double> three() {
    return {1, "two", 3.0};
}

int main() {
    demo::title("C++17 structured bindings");
    // From tuple/pair.
    auto [a, b, c] = three();
    DEMO_ASSERT(a == 1 && b == "two" && c == 3.0);

    // From array.
    std::array<int, 3> arr{10, 20, 30};
    auto [x, y, z] = arr;
    DEMO_ASSERT(x == 10 && y == 20 && z == 30);

    // From a struct (aggregate).
    Point p{4, 5};
    auto [px, py] = p;
    DEMO_ASSERT(px == 4 && py == 5);

    // Bind by reference -- mutate the underlying tuple.
    std::pair<int, int> pr{0, 0};
    auto& [pa, pb] = pr;
    pa = 7;
    DEMO_ASSERT(pr.first == 7);

    // Idiomatic loop over a map.
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};
    int sum = 0;
    for (auto& [key, value] : m) {
        (void)key;
        sum += value;
    }
    DEMO_ASSERT(sum == 3);
    return 0;
}
