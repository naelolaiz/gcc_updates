// gcc-test: std=c++23 min-gcc=15 topic=stl experimental=false
// description: std::flat_set is the set-shaped counterpart of flat_map: sorted contiguous storage instead of an RB tree.
// reference: https://en.cppreference.com/w/cpp/container/flat_set

#include "support/demo.hpp"
#include <cassert>
#include <flat_set>

int main() {
    demo::title("C++23 flat set");
    std::flat_set<int> s;
    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);   // dedup

    DEMO_ASSERT(s.size() == 3);
    DEMO_ASSERT(s.contains(20));
    DEMO_ASSERT(*s.begin() == 10);

    // Order matches std::set, but storage is contiguous.
    int prev = -1;
    for (int v : s) {
        DEMO_ASSERT(v > prev);
        prev = v;
    }
    return 0;
}
