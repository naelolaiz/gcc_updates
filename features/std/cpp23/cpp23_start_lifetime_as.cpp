// description: std::start_lifetime_as<T>(p) formally begins T's lifetime at the bytes pointed to by p without running a constructor -- the standardised, defined-behaviour way to "view a buffer as a T", obsoleting the reinterpret_cast / memcpy dance for trivially-copyable types.
// reference: https://en.cppreference.com/w/cpp/memory/start_lifetime_as

#include "support/demo.hpp"
#include <cstddef>
#include <cstring>
#include <memory>

struct Header {
    std::uint32_t magic;
    std::uint16_t version;
    std::uint16_t flags;
};
static_assert(std::is_trivially_copyable_v<Header>);

int main() {
    demo::title("C++23 start_lifetime_as");

    // Pretend this came off the wire / off disk: it's just bytes that
    // *encode* a Header but aren't yet a Header object as far as the
    // language is concerned.
    alignas(Header) std::byte buffer[sizeof(Header)];
    Header tmp{0xCAFEBABE, 7, 0x0001};
    std::memcpy(buffer, &tmp, sizeof(Header));

    // Pre-C++23: reinterpret_cast<Header*>(buffer)->magic is technically
    // UB (no Header object lives there). The classic workaround was
    // copying back into a Header variable. start_lifetime_as instead
    // *creates* the Header in-place from the existing bytes.
    Header* h = std::start_lifetime_as<Header>(buffer);
    DEMO_ASSERT(h->magic   == 0xCAFEBABE);
    DEMO_ASSERT(h->version == 7);
    DEMO_ASSERT(h->flags   == 0x0001);

    demo::value("magic",   static_cast<int>(h->magic));
    demo::value("version", h->version);
    demo::value("flags",   h->flags);

    // Array form: start_lifetime_as_array<T>(p, n) for buffers of N Ts.
    alignas(int) std::byte ints_buf[sizeof(int) * 3];
    int seed[3] = {10, 20, 30};
    std::memcpy(ints_buf, seed, sizeof(seed));
    int* arr = std::start_lifetime_as_array<int>(ints_buf, 3);
    DEMO_ASSERT(arr[0] == 10 && arr[2] == 30);
    return 0;
}
