// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: Associative containers gained .contains(key) -- clearer than 'count(k) > 0' or 'find(k) != end()'.
// reference: https://en.cppreference.com/w/cpp/container/map/contains

#include <cassert>
#include <map>
#include <set>
#include <unordered_set>

int main() {
    std::map<int, int> m{{1, 100}, {2, 200}};
    assert(m.contains(1));
    assert(!m.contains(3));

    std::set<int> s{10, 20, 30};
    assert(s.contains(20));
    assert(!s.contains(99));

    std::unordered_set<int> us{1, 2, 3};
    assert(us.contains(2));
    assert(!us.contains(4));
    return 0;
}
