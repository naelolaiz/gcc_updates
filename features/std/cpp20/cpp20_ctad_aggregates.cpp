// description: Class template argument deduction now works on aggregates without writing deduction guides by hand.
// reference: https://en.cppreference.com/w/cpp/language/class_template_argument_deduction

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <type_traits>

// Pure aggregate -- no constructor, no deduction guide.
template <typename Key, typename Value>
struct Pair {
    Key   key;
    Value value;
};

template <typename T>
struct Wrapper {
    T contents;
};

int main() {
    demo::title("C++20 ctad aggregates");
    // C++20: deduces Pair<const char*, int> from braced init.
    Pair p{"answer", 42};
    static_assert(std::is_same_v<decltype(p.key),   const char*>);
    static_assert(std::is_same_v<decltype(p.value), int>);
    DEMO_ASSERT(p.value == 42);

    Pair p2{std::string("hi"), 3.14};
    static_assert(std::is_same_v<decltype(p2.key),   std::string>);
    static_assert(std::is_same_v<decltype(p2.value), double>);

    Wrapper w{std::string("hello")};
    static_assert(std::is_same_v<decltype(w.contents), std::string>);
    DEMO_ASSERT(w.contents == "hello");
    return 0;
}
