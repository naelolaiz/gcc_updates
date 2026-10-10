// description: C++23 containers construct from any range with std::from_range and grow with append_range / insert_range / assign_range -- whole ranges go in without an iterator pair.
// reference: https://en.cppreference.com/w/cpp/ranges/from_range
// why: A lazy view can become, or extend, a container in one call even when its iterator and sentinel types differ.
// before: Callers wrote vector(r.begin(), r.end()) (impossible for unequal iterator/sentinel types) or a push_back loop.
// pitfall: The tagged constructor needs std::from_range; braces with a range alone pick the initializer_list constructor instead.

#include "support/demo.hpp"
#include <deque>
#include <list>
#include <ranges>
#include <set>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 from_range construction and *_range insertion");

    // iota(1) | take(4) has a counted sentinel, not an end iterator.
    auto first_four = std::views::iota(1) | std::views::take(4);
    std::vector<int> v(std::from_range, first_four);
    demo::range("vector(from_range, ...)", v);
    DEMO_ASSERT((v == std::vector{1, 2, 3, 4}));

    // Append, insert, and assign whole ranges.
    v.append_range(std::views::repeat(9, 2));
    DEMO_ASSERT((v == std::vector{1, 2, 3, 4, 9, 9}));
    v.insert_range(v.begin(), std::list{-1, 0});
    DEMO_ASSERT((v == std::vector{-1, 0, 1, 2, 3, 4, 9, 9}));

    std::deque<int> d;
    d.prepend_range(std::views::iota(0, 3));
    DEMO_ASSERT((d == std::deque{0, 1, 2}));

    std::string s;
    s.assign_range(std::string_view{"ranges"} | std::views::reverse);
    DEMO_ASSERT(s == "segnar");

    // Associative containers too: duplicates collapse as with insert().
    std::set<int> uniq(std::from_range, std::vector{3, 1, 3, 2});
    DEMO_ASSERT((uniq == std::set{1, 2, 3}));
    return 0;
}
