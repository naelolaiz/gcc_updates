// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: Brace-init {} works uniformly for aggregates, containers, and built-ins; rejects narrowing conversions.
// reference: https://en.cppreference.com/w/cpp/language/list_initialization

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
    int a{42};
    Point p{1, 2};
    std::vector<int> v{1, 2, 3, 4};
    std::map<int, int> m{{1, 10}, {2, 20}};

    assert(a == 42);
    assert(p.x == 1 && p.y == 2);
    assert(v.size() == 4);
    assert(m[1] == 10);

    // Narrowing is rejected at compile time.
    // int narrow{3.14};   // would not compile

    // initializer_list lets functions take {a, b, c, ...} directly.
    assert(sum({1, 2, 3, 4, 5}) == 15);
    return 0;
}
