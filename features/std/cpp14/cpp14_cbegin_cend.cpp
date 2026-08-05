// description: Free functions std::cbegin/cend, std::rbegin/rend and friends work on containers AND raw C arrays -- generic code gets const or reverse iterators without member-function assumptions.
// reference: https://en.cppreference.com/w/cpp/iterator/begin

#include "support/demo.hpp"
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

int main() {
    demo::title("C++14 free cbegin/cend/rbegin/rend");

    std::vector<int> v{1, 2, 3};

    // cbegin on a non-const container yields a const_iterator -- code that
    // must not modify elements can say so at the call site.
    static_assert(std::is_same<decltype(std::cbegin(v)),
                               std::vector<int>::const_iterator>::value,
                  "");
    DEMO_ASSERT(std::count(std::cbegin(v), std::cend(v), 2) == 1);

    // They also work on raw C arrays, where .begin() doesn't exist --
    // one spelling for both worlds.
    int raw[] = {10, 20, 30};
    DEMO_ASSERT(*std::cbegin(raw) == 10);
    DEMO_ASSERT(std::cend(raw) - std::cbegin(raw) == 3);

    // Reverse variants, again container-or-array agnostic.
    std::vector<int> reversed(std::rbegin(raw), std::rend(raw));
    DEMO_ASSERT(reversed.front() == 30);
    demo::range("reversed raw[]", reversed);
    return 0;
}
