// gcc-test: std=c++23 min-gcc=15 topic=stl experimental=false
// description: std::flat_set is the set-shaped counterpart of flat_map: sorted contiguous storage instead of an RB tree.
// reference: https://en.cppreference.com/w/cpp/container/flat_set

#include <cassert>
#include <flat_set>

int main() {
    std::flat_set<int> s;
    s.insert(30);
    s.insert(10);
    s.insert(20);
    s.insert(10);   // dedup

    assert(s.size() == 3);
    assert(s.contains(20));
    assert(*s.begin() == 10);

    // Order matches std::set, but storage is contiguous.
    int prev = -1;
    for (int v : s) {
        assert(v > prev);
        prev = v;
    }
    return 0;
}
