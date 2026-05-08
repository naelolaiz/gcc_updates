// description: Trailing return type 'auto f(...) -> T' enables decltype-based return types that need parameters in scope.
// reference: https://en.cppreference.com/w/cpp/language/function

#include "support/demo.hpp"
#include <cassert>
#include <type_traits>

// The return type depends on the parameter types -- it must come AFTER them.
template <typename A, typename B>
auto add(A a, B b) -> decltype(a + b) { return a + b; }

int main() {
    demo::title("C++11 trailing return");
    auto i = add(1, 2);
    auto d = add(1, 2.5);
    static_assert(std::is_same<decltype(i), int>::value, "");
    static_assert(std::is_same<decltype(d), double>::value, "");
    DEMO_ASSERT(i == 3);
    DEMO_ASSERT(d == 3.5);
    return 0;
}
