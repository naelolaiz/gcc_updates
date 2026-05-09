// description: <charconv>::to_chars / from_chars are locale-free, allocation-free integer (and floating, since libstdc++ 11) conversion -- the fast path for parsing and formatting numbers vs std::stoi / sprintf.
// reference: https://en.cppreference.com/w/cpp/header/charconv

#include "support/demo.hpp"
#include <array>
#include <charconv>
#include <string_view>
#include <system_error>

int main() {
    demo::title("C++17 charconv");

    // Integer formatting: write into a caller-provided buffer; no allocation.
    std::array<char, 32> buf{};
    auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), 12345);
    DEMO_ASSERT(ec == std::errc{});
    std::string_view written(buf.data(), ptr - buf.data());
    demo::text("to_chars(12345)", std::string(written));

    // Hex base argument (binary / octal / hex / arbitrary 2..36).
    auto [hex_end, hex_ec] = std::to_chars(buf.data(), buf.data() + buf.size(),
                                           255, 16);
    DEMO_ASSERT(hex_ec == std::errc{});
    DEMO_ASSERT(std::string_view(buf.data(), hex_end - buf.data()) == "ff");

    // Integer parsing: returns ec=invalid_argument on no digits,
    // ec=result_out_of_range on overflow.
    int parsed = 0;
    std::string_view input = "42xyz";
    auto [next, parse_ec] = std::from_chars(input.data(),
                                            input.data() + input.size(), parsed);
    DEMO_ASSERT(parse_ec == std::errc{});
    DEMO_ASSERT(parsed == 42);
    demo::value("trailing chars left",
                static_cast<int>(input.data() + input.size() - next));

    // Overflow detection -- much easier than catching std::out_of_range from stoi.
    int dummy;
    std::string_view huge = "99999999999999999999";
    auto [_, big_ec] = std::from_chars(huge.data(), huge.data() + huge.size(),
                                       dummy);
    DEMO_ASSERT(big_ec == std::errc::result_out_of_range);
    demo::text("huge", "rejected with errc::result_out_of_range");
    return 0;
}
