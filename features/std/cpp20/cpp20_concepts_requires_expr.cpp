// description: requires-expressions can introspect type capabilities (simple, type, compound, nested requirements).
// reference: https://en.cppreference.com/w/cpp/language/requires

#include "support/demo.hpp"
#include <cassert>
#include <concepts>
#include <type_traits>
#include <vector>

// 1. Simple requirement: expression must be valid.
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

// 2. Type requirement: a nested type must exist.
template <typename T>
concept HasValueType = requires { typename T::value_type; };

// 3. Compound requirement: expression valid AND result satisfies a type constraint.
template <typename T>
concept Sized = requires(const T& t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

// 4. Nested requirement: a constexpr predicate over the satisfied capabilities.
template <typename T>
concept SmallTrivial = requires {
    requires sizeof(T) <= 8;
    requires std::is_trivially_copyable_v<T>;
};

struct NoOps {};

int main() {
    demo::title("C++20 concepts requires expr");
    static_assert(Addable<int>);
    static_assert(!Addable<NoOps>);

    static_assert(HasValueType<std::vector<int>>);
    static_assert(!HasValueType<int>);

    static_assert(Sized<std::vector<int>>);
    static_assert(!Sized<int>);

    static_assert(SmallTrivial<int>);
    static_assert(!SmallTrivial<std::vector<int>>);

    // requires-expression as a runtime bool too.
    constexpr bool int_addable = requires(int a, int b) { a + b; };
    static_assert(int_addable);
    demo::text("check", "static assertions passed");
    return 0;
}
