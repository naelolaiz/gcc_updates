// description: UBSan's vptr check diagnoses a virtual call through storage that does not contain an object of the claimed dynamic type.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// why: A plausible-looking pointer can still lack the dynamic object required for virtual dispatch.
// before: The bad call often surfaced as an opaque jump through a corrupt vtable pointer.
// pitfall: Aligned and sufficiently large storage is still not a constructed Base object.

#include <cstddef>

struct Base {
    virtual ~Base() = default;
    virtual int value() const { return 1; }
};

int main() {
    alignas(Base) std::byte storage[sizeof(Base)]{};
    auto* invalid = reinterpret_cast<Base*>(storage);
    return invalid->value();
}
