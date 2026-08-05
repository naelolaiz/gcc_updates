// description: 'using enum' brings a scoped enum's enumerators into the current scope -- switch cases read 'red' instead of 'Color::red' without giving up enum class type safety.
// reference: https://en.cppreference.com/w/cpp/language/enum

#include "support/demo.hpp"
#include <string>

enum class Color { red, green, blue };

std::string name(Color c) {
    switch (c) {
        using enum Color;  // enumerators visible for the cases below only
        case red:   return "red";
        case green: return "green";
        case blue:  return "blue";
    }
    return "?";
}

// Also allowed at class scope: exposes Palette::red etc.
struct Palette {
    using enum Color;
};

int main() {
    demo::title("C++20 using enum");

    DEMO_ASSERT(name(Color::green) == "green");
    DEMO_ASSERT(Palette::red == Color::red);
    demo::text("name(Color::blue)", name(Color::blue));
    return 0;
}
