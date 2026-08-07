// description: <debugging> provides a portable query for debugger presence and standardized breakpoint operations.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2546r5.html
// why: Debug traps and debugger detection no longer require platform-specific APIs at every call site.
// before: __builtin_trap, DebugBreak, ptrace/procfs checks, or inline assembly.
// pitfall: breakpoint() deliberately interrupts execution, so this demo only exercises the safe query.

#include "support/demo.hpp"
#include <concepts>
#include <debugging>

int main() {
    demo::title("C++26 debugging support");
    static_assert(std::same_as<decltype(std::is_debugger_present()), bool>);
    static_assert(noexcept(std::is_debugger_present()));
    demo::value("debugger present", std::is_debugger_present());
    return 0;
}
