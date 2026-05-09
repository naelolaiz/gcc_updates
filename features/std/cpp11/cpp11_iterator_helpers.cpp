// description: C++11 added free std::begin/std::end (work on raw arrays AND containers), std::next/std::prev (advance an iterator by N without mutation), and std::move_iterator (turns *it into std::move(*it) for moving sequences).
// reference: https://en.cppreference.com/w/cpp/iterator

#include "support/demo.hpp"
#include <iterator>
#include <string>
#include <vector>

int main() {
    demo::title("C++11 iterator helpers");

    // std::begin / std::end are *free functions* and work on raw arrays;
    // generic code can stop carrying both `T*` and `Container` overloads.
    int arr[] = {10, 20, 30, 40};
    DEMO_ASSERT(std::distance(std::begin(arr), std::end(arr)) == 4);

    std::vector<int> v{1, 2, 3, 4, 5};
    DEMO_ASSERT(std::distance(std::begin(v), std::end(v)) == 5);

    // std::next / std::prev: non-mutating advance/retreat. Saves a temporary
    // when you just want "the iterator one past this one".
    auto third = std::next(v.begin(), 2);
    DEMO_ASSERT(*third == 3);
    DEMO_ASSERT(*std::prev(v.end()) == 5);

    // std::move_iterator wraps an iterator so that *it returns an rvalue
    // reference -- copying through it actually moves. Useful for transferring
    // a vector<string> into another container without a string copy each.
    std::vector<std::string> src{"alpha", "beta", "gamma"};
    std::vector<std::string> dst(
        std::make_move_iterator(src.begin()),
        std::make_move_iterator(src.end()));
    DEMO_ASSERT(dst[0] == "alpha");
    // src strings are now in moved-from state (still valid, unspecified value).
    return 0;
}
