// description: UndefinedBehaviorSanitizer diagnoses a typed load through an address that does not meet the type's alignment requirement.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// why: Misaligned typed access may work on one CPU while remaining undefined C++ behavior.
// before: Developers relied on target-specific faults or manually inspected pointer arithmetic.
// pitfall: Enough bytes do not make an address suitably aligned or begin an int lifetime.

#include <cstddef>

int main() {
    alignas(int) std::byte storage[sizeof(int) + 1]{};
    auto* misaligned = reinterpret_cast<int*>(storage + 1);
    return *misaligned;
}
