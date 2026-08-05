// description: Pack indexing: pack...[I] selects the I-th element of a parameter pack directly -- no recursive helpers or tuple detours to reach a single pack element.
// reference: https://en.cppreference.com/w/cpp/language/pack_indexing

#include "support/demo.hpp"
#include <string>
#include <type_traits>

// Value packs: args...[0] is the first argument, no std::get needed.
template <typename... Ts>
constexpr auto first_arg(Ts... args) { return args...[0]; }

template <typename... Ts>
constexpr auto last_arg(Ts... args) { return args...[sizeof...(args) - 1]; }

// Type packs index the same way.
template <typename... Ts>
using second_type = Ts...[1];

int main() {
    demo::title("C++26 pack indexing");

    static_assert(first_arg(7, 8, 9) == 7);
    static_assert(last_arg(7, 8, 9) == 9);
    DEMO_ASSERT(first_arg(std::string("a"), std::string("b")) == "a");

    static_assert(std::is_same_v<second_type<char, int, double>, int>);
    demo::value("last_arg(1, 2, 3)", last_arg(1, 2, 3));
    return 0;
}
