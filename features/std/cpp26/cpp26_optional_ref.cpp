// description: std::optional<T&> represents an optional non-owning reference and writes through to the original object when engaged.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p2988r12.pdf
// why: APIs can return maybe-a-reference without a raw pointer or reference_wrapper ceremony.
// before: T*, optional<reference_wrapper<T>>, or an index/sentinel pair.
// pitfall: The referred object must outlive the optional; reset() does not destroy it.

#include "support/demo.hpp"
#include <optional>
#include <type_traits>

int main() {
    demo::title("C++26 optional reference");
    int value = 7;
    std::optional<int&> ref = value;
    static_assert(std::is_same_v<decltype(*ref), int&>);

    *ref = 42;
    DEMO_ASSERT(value == 42);
    ref.reset();
    DEMO_ASSERT(!ref);
    DEMO_ASSERT(value == 42);
    return 0;
}
