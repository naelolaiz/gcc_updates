// description: std::cmp_less / cmp_equal / in_range compare signed and unsigned integers mathematically -- with plain operator<, -1 converts to a huge unsigned value and the comparison silently lies.
// reference: https://en.cppreference.com/w/cpp/utility/intcmp

#include "support/demo.hpp"
#include <cstdint>
#include <limits>
#include <utility>

int main() {
    demo::title("C++20 safe integer comparisons");

    int neg = -1;
    unsigned one = 1u;

    // The trap these fix: in 'neg < one' the usual arithmetic conversions
    // turn -1 into UINT_MAX, so the naive comparison is false (and GCC
    // rightly warns with -Wsign-compare).
    demo::value("static_cast<unsigned>(-1)", static_cast<unsigned>(neg));
    DEMO_ASSERT(static_cast<unsigned>(neg) == std::numeric_limits<unsigned>::max());

    DEMO_ASSERT(std::cmp_less(neg, one));      // -1 < 1, mathematically
    DEMO_ASSERT(std::cmp_greater(one, neg));
    DEMO_ASSERT(!std::cmp_equal(neg, std::numeric_limits<unsigned>::max()));

    // in_range<T>(v): does the VALUE v fit in type T?
    DEMO_ASSERT(std::in_range<std::uint8_t>(255));
    DEMO_ASSERT(!std::in_range<std::uint8_t>(256));
    DEMO_ASSERT(!std::in_range<unsigned>(-1));
    DEMO_ASSERT(std::in_range<int>(-1));
    return 0;
}
