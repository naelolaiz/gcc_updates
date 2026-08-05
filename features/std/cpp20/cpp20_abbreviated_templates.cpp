// description: Abbreviated function templates: 'auto' in a function parameter list declares a template parameter -- 'void f(auto x)' equals 'template<class T> void f(T x)', and a concept can constrain it in place.
// reference: https://en.cppreference.com/w/cpp/language/function_template

#include "support/demo.hpp"
#include <concepts>
#include <string>
#include <utility>

// Same as: template <typename T> T twice(T x) -- but reads like a function.
auto twice(auto x) { return x + x; }

// Constrained in place: only integral types are accepted.
auto halve(std::integral auto x) { return x / 2; }

// auto&& is a forwarding reference here, exactly as in the long form.
void append(auto& container, auto&& value) {
    container += std::forward<decltype(value)>(value);
}

int main() {
    demo::title("C++20 abbreviated function templates");

    DEMO_ASSERT(twice(21) == 42);
    DEMO_ASSERT(twice(std::string("ab")) == "abab");

    DEMO_ASSERT(halve(10) == 5);
    // halve(1.5);  // compile error: double does not satisfy std::integral

    std::string s = "abc";
    append(s, "def");
    DEMO_ASSERT(s == "abcdef");
    demo::text("s", s);
    return 0;
}
