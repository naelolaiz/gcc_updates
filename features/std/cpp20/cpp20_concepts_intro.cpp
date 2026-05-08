// gcc-test: std=c++20 min-gcc=14 topic=concepts experimental=false
// description: Defining and using concepts to constrain templates -- replaces SFINAE for most cases.
// reference: https://en.cppreference.com/w/cpp/language/constraints

#include "support/demo.hpp"
#include <cassert>
#include <concepts>
#include <string>
#include <type_traits>

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <typename T>
concept Stringy = std::convertible_to<T, std::string>;

template <Numeric T>
constexpr T twice(T x) { return x + x; }

// requires-clause form
template <typename T>
    requires Numeric<T>
constexpr T thrice(T x) { return x + x + x; }

// Abbreviated function template.
constexpr auto plus_one(Numeric auto x) { return x + 1; }

int main() {
    demo::title("C++20 concepts intro");
    static_assert(twice(3) == 6);
    static_assert(twice(2.5) == 5.0);
    static_assert(thrice(4) == 12);
    static_assert(plus_one(7) == 8);

    static_assert(Stringy<const char*>);
    static_assert(Stringy<std::string>);
    static_assert(!Stringy<int>);

    // Runtime sanity check too, in case any of the above constexpr were
    // mis-evaluated by an unexpected non-constexpr fallback.
    DEMO_ASSERT(twice(10) == 20);
    return 0;
}
