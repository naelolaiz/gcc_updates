// description: GCC 16 ships ~two-thirds of C++26 by default (per release notes); this is a smoke test that c++26 mode is healthy.
// reference: https://gcc.gnu.org/gcc-16/changes.html

#include "support/demo.hpp"
#include <cassert>
#include <cstdint>
#include <numeric>

int main() {
    demo::title("GCC 16 cpp26 features default");
    // C++26-era saturation arithmetic, available since GCC 14, kept healthy in GCC 16.
    static_assert(std::add_sat<std::int8_t>(120, 10) == 127);
    demo::text("check", "static assertions passed");
    return 0;
}
