// description: <numeric> in C++26 adds saturation arithmetic -- and shows live draft churn: GCC 14/15 ship add_sat/saturate_cast (__cpp_lib_saturation_arithmetic 202311), GCC 16 the renamed saturating_add/saturating_cast (202603).
// reference: https://en.cppreference.com/w/cpp/numeric
// why: Fixed-width integer arithmetic can clamp at the type's minimum/maximum bounds without signed-overflow UB or unsigned wraparound.
// before: Callers widened to a larger integer type and clamped with std::clamp or used compiler intrinsics.
// pitfall: WG21 renamed *_sat/saturate_cast to saturating_* in 2026-03; check __cpp_lib_saturation_arithmetic when spanning GCC 14-16.

#include "support/demo.hpp"
#include <cassert>
#include <climits>
#include <cstdint>
#include <numeric>
#include <version>

int main() {
    demo::title("C++26 saturation arith");

#if __cpp_lib_saturation_arithmetic >= 202603L
    // Draft revision 2026-03 (libstdc++ 16+): the operations were renamed.
    static_assert(std::saturating_add<std::int8_t>(120, 10) == INT8_MAX);
    static_assert(std::saturating_add<std::int8_t>(-120, -10) == INT8_MIN);

    static_assert(std::saturating_sub<std::uint8_t>(5, 10) == 0);
    static_assert(std::saturating_mul<std::int16_t>(20000, 5) == INT16_MAX);

    // saturating_cast clamps when the value falls outside the target range.
    static_assert(std::saturating_cast<std::int8_t>(1000) == INT8_MAX);
    static_assert(std::saturating_cast<std::int8_t>(-1000) == INT8_MIN);
    static_assert(std::saturating_cast<std::uint8_t>(-1) == 0);
    demo::text("api", "saturating_* (202603, libstdc++ 16+)");
#else
    // Original C++26 wording (libstdc++ 14/15): *_sat / saturate_cast.
    static_assert(std::add_sat<int8_t>(120, 10) == INT8_MAX);
    static_assert(std::add_sat<int8_t>(-120, -10) == INT8_MIN);

    static_assert(std::sub_sat<uint8_t>(5, 10) == 0);
    static_assert(std::mul_sat<int16_t>(20000, 5) == INT16_MAX);

    // saturate_cast clamps when the value falls outside the target range.
    static_assert(std::saturate_cast<int8_t>(1000)  == INT8_MAX);
    static_assert(std::saturate_cast<int8_t>(-1000) == INT8_MIN);
    static_assert(std::saturate_cast<uint8_t>(-1)   == 0);
    demo::text("api", "*_sat / saturate_cast (202311, libstdc++ 14/15)");
#endif

    demo::text("check", "static assertions passed");
    return 0;
}
