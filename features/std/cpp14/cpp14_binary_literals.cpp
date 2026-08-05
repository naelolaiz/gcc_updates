// description: Binary literals (0b1010) and digit separators (1'000'000) -- bit masks readable at a glance, large constants readable without counting zeros.
// reference: https://en.cppreference.com/w/cpp/language/integer_literal

#include "support/demo.hpp"
#include <cstdint>

int main() {
    demo::title("C++14 binary literals + digit separators");

    // Permission bits spelled as bits, not as magic decimal/octal numbers.
    constexpr std::uint8_t read = 0b100;
    constexpr std::uint8_t write = 0b010;
    constexpr std::uint8_t exec = 0b001;
    constexpr std::uint8_t rwx = read | write | exec;
    static_assert(rwx == 7, "");
    DEMO_ASSERT((rwx & write) != 0);

    // Separators are ignored by the compiler; group however reads best.
    constexpr long million = 1'000'000;
    DEMO_ASSERT(million == 1000000);

    // Both features combine nicely for hardware-style masks.
    constexpr std::uint32_t mask = 0b1111'0000'1111'0000;
    constexpr std::uint32_t hex_same = 0xF0F0;
    static_assert(mask == hex_same, "binary and hex spell the same value");

    demo::value("mask", mask);
    demo::value("million", million);
    return 0;
}
