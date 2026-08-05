// description: GCC 16 ships ~two-thirds of C++26 by default and tracks draft revisions live: the saturation API is the renamed saturating_add/saturating_cast (__cpp_lib_saturation_arithmetic 202603), replacing GCC 14/15's add_sat.
// reference: https://gcc.gnu.org/gcc-16/changes.html

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>
#include <numeric>
#include <version>

int main() {
    demo::title("GCC 16 cpp26 features default");

    // libstdc++ 16 follows the 2026-03 draft revision of saturation
    // arithmetic; the older add_sat spelling is gone.
    static_assert(__cpp_lib_saturation_arithmetic >= 202603L);
    static_assert(std::saturating_add<std::int8_t>(120, 10) == 127);
    static_assert(std::saturating_cast<std::uint8_t>(-1) == 0);
    demo::text("check", "static assertions passed");
    return 0;
}
