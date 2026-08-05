// description: __int128 / unsigned __int128 give 128-bit integer arithmetic on 64-bit targets -- full-width 64x64 multiplies without intrinsics (wrap in __extension__ to stay -Wpedantic-clean).
// reference: https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html

#include "support/demo.hpp"
#include <cstdint>
#include <string>

// __extension__ silences the "ISO C++ does not support __int128" pedwarn.
__extension__ typedef unsigned __int128 u128;

// The classic use: exact 64x64 -> 128 multiply, then take the high half.
std::uint64_t mulhi64(std::uint64_t a, std::uint64_t b) {
    return static_cast<std::uint64_t>((static_cast<u128>(a) * b) >> 64);
}

// iostreams can't print __int128; convert by hand for display.
std::string to_string_u128(u128 x) {
    if (x == 0) return "0";
    std::string s;
    while (x > 0) {
        s.insert(s.begin(), static_cast<char>('0' + static_cast<int>(x % 10)));
        x /= 10;
    }
    return s;
}

int main() {
    demo::title("GCC __int128");

    const std::uint64_t big = 0xFFFFFFFFFFFFFFFFull;  // 2^64 - 1
    u128 square = static_cast<u128>(big) * big;       // exact (2^64-1)^2

    // (2^64-1)^2 = 2^128 - 2^65 + 1: high half 2^64-2, low half 1.
    DEMO_ASSERT(mulhi64(big, big) == 0xFFFFFFFFFFFFFFFEull);
    DEMO_ASSERT(static_cast<std::uint64_t>(square) == 1);
    demo::text("(2^64-1)^2", to_string_u128(square));
    return 0;
}
