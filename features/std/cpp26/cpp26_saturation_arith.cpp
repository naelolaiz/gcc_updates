// gcc-test: std=c++26 min-gcc=14 topic=stl experimental=true
// description: <numeric> in C++26 adds saturation arithmetic (add_sat, sub_sat, mul_sat, div_sat) and saturate_cast.
// reference: https://en.cppreference.com/w/cpp/numeric#Saturation_arithmetic

#include <cassert>
#include <climits>
#include <cstdint>
#include <numeric>

int main() {
    // add_sat clamps instead of overflowing.
    static_assert(std::add_sat<int8_t>(120, 10) == INT8_MAX);
    static_assert(std::add_sat<int8_t>(-120, -10) == INT8_MIN);

    static_assert(std::sub_sat<uint8_t>(5, 10) == 0);
    static_assert(std::mul_sat<int16_t>(20000, 5) == INT16_MAX);

    // saturate_cast clamps when the source value falls outside the target range.
    static_assert(std::saturate_cast<int8_t>(1000)  == INT8_MAX);
    static_assert(std::saturate_cast<int8_t>(-1000) == INT8_MIN);
    static_assert(std::saturate_cast<uint8_t>(-1)   == 0);
    return 0;
}
