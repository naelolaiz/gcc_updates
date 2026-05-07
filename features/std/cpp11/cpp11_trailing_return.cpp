// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: Trailing return type 'auto f(...) -> T' enables decltype-based return types that need parameters in scope.
// reference: https://en.cppreference.com/w/cpp/language/function

#include <cassert>
#include <type_traits>

// The return type depends on the parameter types -- it must come AFTER them.
template <typename A, typename B>
auto add(A a, B b) -> decltype(a + b) { return a + b; }

int main() {
    auto i = add(1, 2);
    auto d = add(1, 2.5);
    static_assert(std::is_same<decltype(i), int>::value, "");
    static_assert(std::is_same<decltype(d), double>::value, "");
    assert(i == 3);
    assert(d == 3.5);
    return 0;
}
