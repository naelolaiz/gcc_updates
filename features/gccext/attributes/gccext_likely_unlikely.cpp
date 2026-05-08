// description: [[likely]] and [[unlikely]] (C++20 standard) hint branch frequency to the optimizer; GCC implements them via __builtin_expect.
// reference: https://en.cppreference.com/w/cpp/language/attributes/likely

#include "support/demo.hpp"
#include <cassert>

int sat_add(int a, int b) {
    if (a > 0 && b > __INT_MAX__ - a) [[unlikely]] return __INT_MAX__;
    if (a < 0 && b < (-__INT_MAX__ - 1) - a) [[unlikely]] return -__INT_MAX__ - 1;
    return a + b;
}

int main() {
    demo::title("GCC extension likely unlikely");
    DEMO_ASSERT(sat_add(1, 2) == 3);
    DEMO_ASSERT(sat_add(__INT_MAX__, 5) == __INT_MAX__);
    DEMO_ASSERT(sat_add(-__INT_MAX__ - 1, -5) == -__INT_MAX__ - 1);
    return 0;
}
