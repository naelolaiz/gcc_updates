// gcc-test: std=c++17 min-gcc=13 topic=gcc-attributes experimental=false
// description: [[gnu::hot]] / [[gnu::cold]] tell the optimizer how often a function runs; influences inlining and code layout.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html

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
    int s = 0;
    for (int i = 0; i < 100; ++i) s += hot_path(i);
    assert(s > 0);
    // rare_failure() never called -- compiler may relocate it to a cold section.
    return 0;
}
