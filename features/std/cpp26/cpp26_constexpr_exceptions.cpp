// description: C++26 permits exceptions during constant evaluation when the exception and its copies are destroyed before evaluation completes.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3068r5.html
// why: One constexpr algorithm can use ordinary exception-based error handling at compile time and runtime.
// before: Sentinel values, optional/expected, or separate constant-evaluation branches.
// pitfall: An exception escaping the constant expression still makes it non-constant.

#include "support/demo.hpp"

constexpr int recover(bool valid) {
    try {
        if (!valid) {
            throw 7;
        }
        return 42;
    } catch (int error) {
        return error;
    }
}

int main() {
    demo::title("C++26 constexpr exceptions");
    static_assert(recover(true) == 42);
    static_assert(recover(false) == 7);
    DEMO_ASSERT(recover(false) == 7);
    return 0;
}
