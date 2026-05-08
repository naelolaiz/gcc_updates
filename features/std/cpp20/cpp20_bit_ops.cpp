// description: <bit> standardises common bit tricks: popcount, countl_zero, has_single_bit, bit_ceil, bit_cast.
// reference: https://en.cppreference.com/w/cpp/header/bit

#include "support/demo.hpp"
#include <bit>
#include <cassert>
#include <cstdint>

int main() {
    demo::title("C++20 bit ops");
    static_assert(std::popcount(0b10110101u) == 5);
    static_assert(std::countl_zero(uint8_t{0b00001000}) == 4);
    static_assert(std::countr_zero(uint8_t{0b00010000}) == 4);

    static_assert(std::has_single_bit(64u));
    static_assert(!std::has_single_bit(60u));

    static_assert(std::bit_ceil(33u)  == 64u);
    static_assert(std::bit_floor(33u) == 32u);
    static_assert(std::bit_width(33u) == 6);

    // bit_cast: type-pun without UB. Reinterpret the bits of a float as an int.
    constexpr float f = 1.0f;
    constexpr std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
    static_assert(bits == 0x3f800000u);

    demo::text("check", "static assertions passed");
    return 0;
}
