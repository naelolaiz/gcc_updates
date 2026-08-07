// description: std::copyable_function owns a copyable type-erased callable while std::function_ref is a lightweight non-owning view of a callable.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2548r6.pdf
// why: Callable ownership and callable observation now have distinct vocabulary types.
// before: std::function was used for both roles, often causing an unnecessary allocation and copy.
// pitfall: function_ref never extends the lifetime of the callable it references.

#include "support/demo.hpp"
#include <functional>

int apply_twice(std::function_ref<int(int)> operation, int value) {
    return operation(operation(value));
}

int main() {
    demo::title("C++26 callable wrappers");

    std::copyable_function<int(int)> owned = [factor = 3](int value) {
        return factor * value;
    };
    auto owned_copy = owned;
    DEMO_ASSERT(owned(4) == 12);
    DEMO_ASSERT(owned_copy(5) == 15);

    auto increment = [](int value) { return value + 1; };
    DEMO_ASSERT(apply_twice(increment, 40) == 42);
    return 0;
}
