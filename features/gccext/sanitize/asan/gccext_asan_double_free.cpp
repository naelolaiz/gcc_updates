// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=address expect-exit=1
// description: ASan demo: freeing the same pointer twice. Runs only under -fsanitize=address; ASan reports 'attempting double-free'.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include <cstdlib>

// Noinline wrapper prevents the optimizer from eliminating the second free as dead code.
[[gnu::noinline]] static void release(void* p) { std::free(p); }

int main() {
    auto* p = std::malloc(16);
    release(p);
    release(p);    // ASan: attempting double-free
    return 0;
}
