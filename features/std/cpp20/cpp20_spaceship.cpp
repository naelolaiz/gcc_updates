// gcc-test: std=c++20 min-gcc=14 topic=language experimental=false
// description: operator<=> ("spaceship") generates the full set of relational operators from one declaration.
// reference: https://en.cppreference.com/w/cpp/language/default_comparisons

#include "support/demo.hpp"
#include <cassert>
#include <compare>

struct Version {
    int major;
    int minor;
    int patch;

    // = default writes <, <=, >, >=, ==, != in one go.
    auto operator<=>(const Version&) const = default;
};

int main() {
    demo::title("C++20 spaceship");
    Version a{1, 2, 3};
    Version b{1, 2, 4};
    Version c{1, 2, 3};

    DEMO_ASSERT(a < b);
    DEMO_ASSERT(b > a);
    DEMO_ASSERT(a == c);
    DEMO_ASSERT(a != b);
    DEMO_ASSERT(a <= c);
    DEMO_ASSERT(b >= a);

    // Three-way result: strong_ordering.
    auto cmp = a <=> b;
    DEMO_ASSERT(cmp == std::strong_ordering::less);
    return 0;
}
