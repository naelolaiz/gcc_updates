// gcc-test: std=c++20 min-gcc=14 topic=templates experimental=false
// description: explicit(bool) makes a constructor's explicitness depend on a constexpr predicate.
// reference: https://en.cppreference.com/w/cpp/language/explicit

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <type_traits>

template <typename T>
struct Wrapper {
    T value;

    // Implicit when T is convertible to int; explicit otherwise.
    explicit(!std::is_convertible_v<T, int>)
        Wrapper(T v) : value(std::move(v)) {}
};

int main() {
    demo::title("C++20 explicit bool");
    Wrapper<int> a = 42;             // implicit -- int is convertible to int
    DEMO_ASSERT(a.value == 42);

    // Wrapper<std::string> b = "hello";   // would NOT compile: explicit
    Wrapper<std::string> c{std::string("hello")};
    DEMO_ASSERT(c.value == "hello");

    static_assert(std::is_convertible_v<int, Wrapper<int>>);
    static_assert(!std::is_convertible_v<std::string, Wrapper<std::string>>);
    return 0;
}
