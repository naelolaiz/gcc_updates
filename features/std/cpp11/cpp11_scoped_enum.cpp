// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'enum class' (scoped enums) don't leak names into the surrounding scope and don't implicitly convert to int.
// reference: https://en.cppreference.com/w/cpp/language/enum

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>
#include <type_traits>

enum class Color : std::uint8_t { Red = 1, Green = 2, Blue = 4 };
enum LegacyColor              { LRed,    LGreen,  LBlue };

int main() {
    demo::title("C++11 scoped enum");
    Color c = Color::Red;

    // No implicit conversion to int.
    // int n = c;          // would not compile
    int n = static_cast<int>(c);
    DEMO_ASSERT(n == 1);

    // Underlying type is fixed; size is one byte.
    static_assert(std::is_same<std::underlying_type<Color>::type, std::uint8_t>::value, "");
    static_assert(sizeof(Color) == 1, "");

    // Names are scoped: 'Red' alone refers to the legacy enum, not Color.
    LegacyColor lc = LRed;
    DEMO_ASSERT(lc == 0);
    return 0;
}
