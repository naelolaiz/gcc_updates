// description: Variable templates: a templated constant like pi<T> instantiates per type -- the mechanism behind C++17's _v trait shorthands (std::is_same_v etc.).
// reference: https://en.cppreference.com/w/cpp/language/variable_template

#include "support/demo.hpp"
#include <type_traits>

template <typename T>
constexpr T pi = T(3.141592653589793238462643L);

// The exact pattern the standard library adopted in C++17 for every
// trait: a '_v' variable template over the '::value' member.
template <typename A, typename B>
constexpr bool is_same_v = std::is_same<A, B>::value;

// Variable templates can be specialised like any other template.
template <unsigned N>
constexpr unsigned long long factorial = N * factorial<N - 1>;
template <>
constexpr unsigned long long factorial<0> = 1;

int main() {
    demo::title("C++14 variable templates");

    static_assert(pi<int> == 3, "instantiated (and truncated) per type");
    DEMO_ASSERT(pi<double> > 3.14159 && pi<double> < 3.1416);
    demo::value("pi<float>", pi<float>);
    demo::value("pi<int>", pi<int>);

    static_assert(is_same_v<int, int> && !is_same_v<int, long>, "");
    static_assert(factorial<10> == 3628800ULL, "recursive variable template");
    demo::value("factorial<10>", factorial<10>);
    return 0;
}
