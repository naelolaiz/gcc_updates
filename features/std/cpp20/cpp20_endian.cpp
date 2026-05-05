// gcc-test: std=c++20 min-gcc=13 topic=stl experimental=false
// description: std::endian exposes the host byte order at compile time -- portable replacement for compiler-specific macros.
// reference: https://en.cppreference.com/w/cpp/types/endian

#include <bit>
#include <cassert>

int main() {
    constexpr bool host_is_little = (std::endian::native == std::endian::little);
    constexpr bool host_is_big    = (std::endian::native == std::endian::big);

    // Exactly one is true on every common architecture.
    assert(host_is_little ^ host_is_big);

    // Use it to dispatch at compile time.
    if constexpr (std::endian::native == std::endian::little) {
        // little-endian path
        assert(true);
    } else {
        // big-endian path
        assert(true);
    }
    return 0;
}
