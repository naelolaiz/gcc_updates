// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=thread expect-exit=66
// description: TSan demo: two threads writing the same plain int without synchronisation. Runs only under -fsanitize=thread; reports a data race.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// note: TSan default exit code on race-on-error is 66 (set via TSAN_OPTIONS=halt_on_error=1). TSan is INCOMPATIBLE with ASan -- run in a separate sanitizer profile.

#include "support/demo.hpp"
#include <thread>

int shared = 0;

[[gnu::noinline]] void hammer() {
    for (int i = 0; i < 100000; ++i) shared = i;   // unsynchronised write
}

int main() {
    demo::title("GCC extension tsan data race");
    std::thread a(hammer);
    std::thread b(hammer);
    a.join();
    b.join();
    return 0;
}
