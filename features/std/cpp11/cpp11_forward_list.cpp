// description: std::forward_list is a singly-linked list -- O(1) splice/insert_after, no .size(), only forward iteration. Use when you really need a singly-linked list and not a vector or list.
// reference: https://en.cppreference.com/w/cpp/container/forward_list

#include "support/demo.hpp"
#include <forward_list>

int main() {
    demo::title("C++11 forward_list");

    // No push_back / no size(); insertion is *_after the cursor.
    std::forward_list<int> fl{1, 2, 4, 5};
    auto pos = fl.begin();           // points at 1
    fl.insert_after(pos, 3);         // 1, 3, 2, 4, 5  (insert AFTER 1)
    DEMO_ASSERT(*std::next(fl.begin(), 1) == 3);

    // The classic use: O(1) splice between two lists, transferring without
    // any element copy/move.
    std::forward_list<int> a{10, 20};
    std::forward_list<int> b{30, 40};
    a.splice_after(a.before_begin(), b);   // prepend b in front of a
    demo::range("after splice_after", a);  // 30, 40, 10, 20

    // remove / remove_if are container methods (the algorithm form would
    // require erase, which forward_list doesn't have -- only erase_after).
    std::forward_list<int> nums{1, 2, 3, 4, 5, 6};
    nums.remove_if([](int x) { return x % 2 == 0; });
    demo::range("odd only", nums);    // 1, 3, 5
    return 0;
}
