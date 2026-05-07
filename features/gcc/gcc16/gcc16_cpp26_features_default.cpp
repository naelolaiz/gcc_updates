// gcc-test: std=c++26 min-gcc=16 topic=gcc-release experimental=true
// description: GCC 16 ships ~two-thirds of C++26 by default (per release notes); this is a smoke test that c++26 mode is healthy.
// reference: https://gcc.gnu.org/gcc-16/changes.html

#include <cassert>
#include <numeric>

int main() {
    // C++26-era saturation arithmetic, available since GCC 14, kept healthy in GCC 16.
    static_assert(std::add_sat<int8_t>(120, 10) == 127);
    return 0;
}
