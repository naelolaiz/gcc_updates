// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::clamp(v, lo, hi) returns max(lo, min(v, hi)); avoids the if/else dance.
// reference: https://en.cppreference.com/w/cpp/algorithm/clamp

#include <algorithm>
#include <cassert>
#include <functional>

int main() {
    assert(std::clamp(5, 1, 10) == 5);
    assert(std::clamp(-3, 0, 10) == 0);
    assert(std::clamp(99, 0, 10) == 10);

    // Custom comparator: case-insensitive char clamp via plain int comparison still applies.
    assert(std::clamp(15, 0, 100, std::greater<>{}) == 100);   // "<= lo" with reversed cmp pins to lo

    static_assert(std::clamp(7, 0, 10) == 7);
    return 0;
}
