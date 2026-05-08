// description: ASan demo: freeing the same pointer twice. Runs only under -fsanitize=address; ASan reports 'attempting double-free'.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include "support/demo.hpp"
#include <cstdlib>

// Noinline wrapper prevents the optimizer from eliminating the second free as dead code.
[[gnu::noinline]] static void release(void* p) { std::free(p); }

int main() {
    demo::title("GCC extension asan double free");
    auto* p = std::malloc(16);
    release(p);
    release(p);    // ASan: attempting double-free
    return 0;
}
