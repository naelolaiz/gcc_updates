// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::byteswap reverses the byte order of an integer; constexpr-friendly endian conversion.
// reference: https://en.cppreference.com/w/cpp/numeric/byteswap

#include <bit>
#include <cassert>
#include <cstdint>

int main() {
    static_assert(std::byteswap<uint16_t>(0x1234) == 0x3412);
    static_assert(std::byteswap<uint32_t>(0x12345678) == 0x78563412);
    static_assert(std::byteswap<uint64_t>(0x0123456789ABCDEF) == 0xEFCDAB8967452301);

    // Pair with std::endian for portable big-endian -> host conversion.
    auto host_from_be = [](uint32_t be) {
        if constexpr (std::endian::native == std::endian::little) {
            return std::byteswap(be);
        } else {
            return be;
        }
    };
    uint32_t observed = host_from_be(0x12345678);
    assert(observed == 0x78563412 || observed == 0x12345678);
    return 0;
}
