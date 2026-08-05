// description: static_assert accepts a constexpr string-like object (anything with constexpr size()/data()) as its message -- diagnostics can be computed instead of being string literals.
// reference: https://en.cppreference.com/w/cpp/language/static_assert

#include "support/demo.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

// A tiny constexpr message builder: the message object only needs
// constexpr size() and data().
struct FixedMsg {
    std::array<char, 64> chars{};
    std::size_t len = 0;
    constexpr void append(const char* s) {
        while (*s != '\0' && len < chars.size()) chars[len++] = *s++;
    }
    constexpr std::size_t size() const { return len; }
    constexpr const char* data() const { return chars.data(); }
};

constexpr FixedMsg layout_msg() {
    FixedMsg m;
    m.append("Packet layout drifted: ");
    m.append("check field packing before changing serialisation");
    return m;
}

struct Packet {
    std::uint32_t id;
    std::uint32_t crc;
};

// On failure the compiler prints the COMPUTED message, not a literal.
static_assert(sizeof(Packet) == 8 && alignof(Packet) == 4, layout_msg());

int main() {
    demo::title("C++26 user-generated static_assert messages");

    DEMO_ASSERT(sizeof(Packet) == 8);
    demo::text("status", "static_assert with a computed message compiled");
    return 0;
}
