// description: User-defined literals: 'value_suffix' calls operator"" _suffix(value). User suffixes must start with an underscore.
// reference: https://en.cppreference.com/w/cpp/language/user_literal

#include "support/demo.hpp"
#include <cassert>
#include <cstddef>

constexpr long double operator"" _km (long double v) { return v * 1000.0L; }
constexpr long double operator"" _mi (long double v) { return v * 1609.344L; }

// String-literal UDL: signature is (const char*, size_t). C++11 constexpr
// forbids loops, so we recurse instead.
constexpr unsigned long long _bin_impl(const char* s, std::size_t n,
                                        unsigned long long acc) {
    return n == 0 ? acc : _bin_impl(s + 1, n - 1, acc * 2 + (*s - '0'));
}
constexpr unsigned long long operator"" _bin(const char* s, std::size_t n) {
    return _bin_impl(s, n, 0);
}

int main() {
    demo::title("C++11 user defined literal");
    static_assert(2.0_km == 2000.0L, "");
    static_assert(static_cast<long long>(1.0_mi) == 1609, "");

    static_assert("1010"_bin == 10, "");
    static_assert("11111111"_bin == 255, "");

    long double total_meters = 1.0_km + 1.0_mi;
    DEMO_ASSERT(total_meters > 2600.0L && total_meters < 2620.0L);
    return 0;
}
