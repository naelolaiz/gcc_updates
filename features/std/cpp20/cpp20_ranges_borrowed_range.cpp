// description: borrowed_range explains why an algorithm result from a temporary can be an iterator for span but std::ranges::dangling for an owning vector.
// reference: https://en.cppreference.com/w/cpp/ranges/borrowed_range
// why: Range algorithms prevent accidental iterators into destroyed temporary containers.
// before: The caller had to audit the lifetime of every returned iterator manually.
// pitfall: A view is non-owning unless its type explicitly says otherwise.

#include "support/demo.hpp"
#include <algorithm>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

int main() {
    demo::title("C++20 borrowed ranges");

    using TemporaryVectorResult = decltype(std::ranges::find(std::vector{1, 2, 3}, 2));
    static_assert(std::same_as<TemporaryVectorResult, std::ranges::dangling>);

    int values[]{1, 2, 3};
    using BorrowedSpan = std::span<int, 3>;
    auto found = std::ranges::find(BorrowedSpan{values}, 2);
    static_assert(std::same_as<decltype(found), BorrowedSpan::iterator>);
    DEMO_ASSERT(*found == 2);
    return 0;
}
