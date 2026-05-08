// description: std::launder<T>(p) tells the compiler "the bytes at p now hold a different T -- don't reuse cached info from the previous T at the same address". Needed when reusing storage across different types of object, especially with const/reference members.
// reference: https://en.cppreference.com/w/cpp/utility/launder

#include "support/demo.hpp"
#include <new>
#include <type_traits>

struct Frozen {
    const int id;
    explicit Frozen(int v) : id(v) {}
};

int main() {
    demo::title("C++17 std::launder");

    // Storage we'll reuse across two distinct Frozen objects.
    alignas(Frozen) unsigned char storage[sizeof(Frozen)];

    // Construct first object.
    Frozen* first = new (storage) Frozen(1);
    DEMO_ASSERT(first->id == 1);
    first->~Frozen();

    // Construct a *second* Frozen at the same address. Because Frozen has a
    // const data member, simply re-reading through `first` is undefined
    // behaviour -- the compiler is allowed to assume id never changes for a
    // given object identity. std::launder hands you a fresh pointer that
    // forgets that assumption.
    Frozen* second_raw = new (storage) Frozen(2);
    Frozen* second = std::launder(second_raw);
    DEMO_ASSERT(second->id == 2);
    second->~Frozen();

    // Other typical use: getting a typed pointer back from a byte buffer
    // returned by an allocator that std::aligned_alloc'd.
    static_assert(std::is_same_v<decltype(std::launder<Frozen>(second)), Frozen*>);
    demo::text("rule of thumb",
               "needed after placement-new replacing a const/ref-having object");
    return 0;
}
