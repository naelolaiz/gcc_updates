// description: std::byte is an enum-class-shaped distinct byte type -- byte ops without unsigned char's arithmetic baggage.
// reference: https://en.cppreference.com/w/cpp/types/byte

#include "support/demo.hpp"
#include <cassert>
#include <cstddef>

int main() {
    demo::title("C++17 byte");
    std::byte b{0b1100'0011};

    // Bitwise ops are defined on std::byte.
    auto andv = b & std::byte{0b0000'1111};
    auto orv  = b | std::byte{0b0000'0000};
    auto xorv = b ^ std::byte{0b1111'1111};
    auto shl  = b << 1;

    DEMO_ASSERT(std::to_integer<int>(andv) == 0b0000'0011);
    DEMO_ASSERT(std::to_integer<int>(orv)  == 0b1100'0011);
    DEMO_ASSERT(std::to_integer<int>(xorv) == 0b0011'1100);
    DEMO_ASSERT(std::to_integer<int>(shl) == ((0b1100'0011 << 1) & 0xff));

    // std::byte forbids accidental arithmetic ('+' won't compile), so misusing
    // it as a number is harder than with unsigned char.
    return 0;
}
