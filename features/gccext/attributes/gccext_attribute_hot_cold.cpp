// description: [[gnu::hot]] / [[gnu::cold]] tell the optimizer how often a function runs; influences inlining and code layout.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

#include "support/demo.hpp"
#include <cassert>

[[gnu::hot]]
int hot_path(int n) {
    int s = 0;
    for (int i = 0; i < n; ++i) s += i;
    return s;
}

[[gnu::cold]]
[[noreturn]] void rare_failure(const char* msg) {
    (void)msg;
    __builtin_trap();
}

int main() {
    demo::title("GCC extension attribute hot cold");
    int s = 0;
    for (int i = 0; i < 100; ++i) s += hot_path(i);
    DEMO_ASSERT(s > 0);
    // rare_failure() never called -- compiler may relocate it to a cold section.
    return 0;
}
