// description: <type_traits> is the C++11 metaprogramming toolbox -- compile-time type queries (is_*) and type transformations (remove_*, decay, conditional, enable_if).
// reference: https://en.cppreference.com/w/cpp/header/type_traits

#include "support/demo.hpp"
#include <type_traits>

// Type queries: each is_*<T>::value is a constexpr bool usable in static_assert,
// SFINAE, or tag dispatch.
struct Base {};
struct Derived : Base {};

static_assert(std::is_integral<int>::value, "");
static_assert(std::is_floating_point<double>::value, "");
static_assert(std::is_arithmetic<char>::value, "");
static_assert(std::is_pointer<int*>::value, "");
static_assert(std::is_const<const int>::value, "");
static_assert(std::is_reference<int&>::value, "");
static_assert(std::is_class<Base>::value, "");
static_assert(std::is_base_of<Base, Derived>::value, "");
static_assert(std::is_convertible<Derived*, Base*>::value, "");
static_assert(std::is_same<int, signed int>::value, "");

// Type transformations strip or add qualifiers; ::type names the result.
static_assert(std::is_same<std::remove_const<const int>::type, int>::value, "");
static_assert(std::is_same<std::remove_reference<int&>::type, int>::value, "");
static_assert(std::is_same<std::remove_pointer<int*>::type, int>::value, "");
static_assert(std::is_same<std::add_pointer<int>::type, int*>::value, "");
static_assert(std::is_same<std::decay<const int&>::type, int>::value, "");
static_assert(std::is_same<std::decay<int[4]>::type, int*>::value, "");

// std::conditional<bool, T, F>::type picks one of two types at compile time.
template <typename T>
using BiggestEnough = typename std::conditional<(sizeof(T) >= 4), T, long>::type;
static_assert(std::is_same<BiggestEnough<char>, long>::value, "");
static_assert(std::is_same<BiggestEnough<int>, int>::value, "");

// std::enable_if turns "this overload exists only for arithmetic T" into a
// SFINAE failure for everything else -- the classic pre-concepts constraint.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
half(T x) {
    return x / 2;
}

int main() {
    demo::title("C++11 type traits");
    DEMO_ASSERT(half(10) == 5);
    DEMO_ASSERT(half(7) == 3);
    // half(2.5);  // would not compile: enable_if removes the overload for double.

    demo::value("is_integral<int>", std::is_integral<int>::value);
    demo::value("is_base_of<Base, Derived>", std::is_base_of<Base, Derived>::value);
    demo::text("decay<const int&>", "int (cv/ref stripped, like by-value parameter)");
    return 0;
}
