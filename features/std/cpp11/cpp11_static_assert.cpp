// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: static_assert(cond, "msg") is a compile-time assertion; in C++17+ the message is optional.
// reference: https://en.cppreference.com/w/cpp/language/static_assert

#include "support/demo.hpp"
#include <type_traits>

template <typename T>
T add_only_arithmetic(T a, T b) {
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic");
    return a + b;
}

int main() {
    demo::title("C++11 static assert");
    static_assert(sizeof(int) >= 2, "int must be at least 16 bits");
    static_assert(2 + 2 == 4, "math is broken");

    int s = add_only_arithmetic(2, 3);
    (void)s;
    // add_only_arithmetic(std::string("a"), std::string("b"));  // would fail static_assert
    demo::text("check", "static assertions passed");
    return 0;
}
