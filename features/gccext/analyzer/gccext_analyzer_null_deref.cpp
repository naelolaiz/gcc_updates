// gcc-test: std=c++17 min-gcc=16 topic=gcc-analyzer experimental=false requires-analyzer=true
// description: -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html

// Compiled under -fanalyzer only; binary is never executed.
// Expect a -Wanalyzer-null-dereference diagnostic on the path where allocate() returns nullptr.

#include "support/demo.hpp"
#include <cstddef>
#include <new>

[[gnu::noinline]] int* allocate(bool ok) {
    return ok ? new int(7) : nullptr;
}

int main(int argc, char**) {
    demo::title("GCC extension analyzer null deref");
    // The analyzer reasons about the call site: when argc <= 1, allocate()
    // returns nullptr and the next line dereferences it.
    int* p = allocate(argc > 1);
    return *p;   // analyzer: null pointer dereference on the argc<=1 path
}
