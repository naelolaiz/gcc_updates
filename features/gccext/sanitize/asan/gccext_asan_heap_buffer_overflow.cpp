// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=address expect-exit=1
// description: ASan demo: writing one past the end of a heap buffer. Runs only under -fsanitize=address; ASan reports 'heap-buffer-overflow'.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include "support/demo.hpp"
#include <cstdlib>

int main() {
    demo::title("GCC extension asan heap buffer overflow");
    auto* p = static_cast<int*>(std::malloc(sizeof(int) * 4));
    p[3] = 0;
    p[4] = 99;          // ASan: heap-buffer-overflow (one past end)
    int v = p[3];
    std::free(p);
    return v;
}
