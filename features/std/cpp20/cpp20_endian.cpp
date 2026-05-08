// description: std::endian exposes the host byte order at compile time -- portable replacement for compiler-specific macros.
// reference: https://en.cppreference.com/w/cpp/types/endian

#include "support/demo.hpp"
#include <bit>
#include <cassert>

int main() {
    demo::title("C++20 endian");
    constexpr bool host_is_little = (std::endian::native == std::endian::little);
    constexpr bool host_is_big    = (std::endian::native == std::endian::big);

    // Exactly one is true on every common architecture.
    DEMO_ASSERT(host_is_little ^ host_is_big);

    // Use it to dispatch at compile time.
    if constexpr (std::endian::native == std::endian::little) {
        // little-endian path
        DEMO_ASSERT(true);
    } else {
        // big-endian path
        DEMO_ASSERT(true);
    }
    return 0;
}
