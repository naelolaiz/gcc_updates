// description: <debugging> provides a portable query for debugger presence and standardized breakpoint operations.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2546r5.html
// why: Debug traps and debugger detection no longer require platform-specific APIs at every call site.
// before: __builtin_trap, DebugBreak, ptrace/procfs checks, or inline assembly.
// pitfall: breakpoint() traps unconditionally; breakpoint_if_debugging() is the form safe to leave in code that may run without a debugger.

#include "support/demo.hpp"
#include <concepts>
#include <debugging>

int main() {
    demo::title("C++26 debugging support");
    static_assert(std::same_as<decltype(std::is_debugger_present()), bool>);
    static_assert(noexcept(std::is_debugger_present()));
    demo::value("debugger present", std::is_debugger_present());

    // A no-op unless a debugger is attached, in which case it stops here.
    std::breakpoint_if_debugging();
    demo::text("breakpoint_if_debugging", "returned");
    return 0;
}
