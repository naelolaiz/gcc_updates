// description: LSan demo: heap allocation with no matching free. Runs only when ASan or LSan is active; reports leaks at exit.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// note: ASan integrates LSan; -fsanitize=address with ASAN_OPTIONS=detect_leaks=1 is enough. Pure -fsanitize=leak also works.

#include "support/demo.hpp"
#include <cstdlib>

[[gnu::noinline]] void* allocate_and_drop() {
    return std::malloc(64);   // intentionally not freed
}

int main() {
    demo::title("GCC extension lsan simple leak");
    (void)allocate_and_drop();
    // Program exits cleanly; LSan scans live heap at exit and reports the leak.
    return 0;
}
