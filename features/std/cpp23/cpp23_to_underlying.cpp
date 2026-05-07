// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::to_underlying returns the underlying-type value of an enum -- replaces the static_cast boilerplate.
// reference: https://en.cppreference.com/w/cpp/utility/to_underlying

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>

enum class Color : uint8_t { Red = 1, Green = 2, Blue = 4 };

int main() {
    demo::title("C++23 to underlying");
    constexpr auto r = std::to_underlying(Color::Red);
    static_assert(std::is_same_v<decltype(r), const std::uint8_t>);
    static_assert(r == 1);

    Color c = Color::Blue;
    auto v = std::to_underlying(c);
    static_assert(std::is_same_v<decltype(v), std::uint8_t>);
    DEMO_ASSERT(v == 4);
    return 0;
}
