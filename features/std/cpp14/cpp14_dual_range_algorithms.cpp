// description: C++14 gave std::equal, std::mismatch, and std::is_permutation overloads taking BOTH ranges' ends -- different-length sequences compare correctly instead of reading past the shorter one.
// reference: https://en.cppreference.com/w/cpp/algorithm/equal

#include "support/demo.hpp"
#include <algorithm>
#include <vector>

int main() {
    demo::title("C++14 dual-range algorithms");

    std::vector<int> a{1, 2, 3};
    std::vector<int> longer{1, 2, 3, 4};

    // The C++11 3-iterator equal assumes the second range is at least as
    // long as the first; the 4-iterator form compares lengths first, so a
    // prefix no longer counts as "equal" and nothing is read out of bounds.
    DEMO_ASSERT(!std::equal(a.begin(), a.end(), longer.begin(), longer.end()));
    DEMO_ASSERT(std::equal(a.begin(), a.end(), longer.begin(), longer.begin() + 3));

    // mismatch: find where two ranges diverge, without overrunning either.
    std::vector<int> b{1, 2, 9, 9};
    auto diff = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
    DEMO_ASSERT(diff.first - a.begin() == 2);
    DEMO_ASSERT(*diff.second == 9);

    // is_permutation: same elements in any order -- length checked too.
    std::vector<int> shuffled{3, 1, 2};
    DEMO_ASSERT(std::is_permutation(a.begin(), a.end(),
                                    shuffled.begin(), shuffled.end()));
    DEMO_ASSERT(!std::is_permutation(a.begin(), a.end(),
                                     longer.begin(), longer.end()));
    demo::range("a", a);
    demo::range("shuffled", shuffled);
    return 0;
}
