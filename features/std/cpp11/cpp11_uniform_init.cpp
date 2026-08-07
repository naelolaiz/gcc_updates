// description: Brace-init {} works uniformly for aggregates, containers, and built-ins; rejects narrowing conversions.
// reference: https://en.cppreference.com/w/cpp/language/list_initialization
// why: One syntax initializes scalars, aggregates, and containers while rejecting silent narrowing.
// before: Parentheses, assignment, and aggregate braces had different rules and failure modes.
// pitfall: An initializer_list constructor can take priority over another seemingly better overload.

#include "support/demo.hpp"
#include <cassert>
#include <initializer_list>
#include <map>
#include <vector>

struct Point { int x; int y; };

int sum(std::initializer_list<int> il) {
    int s = 0;
    for (int v : il) s += v;
    return s;
}

int main() {
    demo::title("C++11 uniform init");
    int a{42};
    Point p{1, 2};
    std::vector<int> v{1, 2, 3, 4};
    std::map<int, int> m{{1, 10}, {2, 20}};

    DEMO_ASSERT(a == 42);
    DEMO_ASSERT(p.x == 1 && p.y == 2);
    DEMO_ASSERT(v.size() == 4);
    DEMO_ASSERT(m[1] == 10);

    // Narrowing is rejected at compile time.
    // int narrow{3.14};   // would not compile

    // initializer_list lets functions take {a, b, c, ...} directly.
    DEMO_ASSERT(sum({1, 2, 3, 4, 5}) == 15);
    return 0;
}
