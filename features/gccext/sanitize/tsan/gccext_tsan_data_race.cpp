// description: TSan demo: two threads writing the same plain int without synchronisation. Runs only under -fsanitize=thread; reports a data race.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// note: TSan is INCOMPATIBLE with ASan, so CI runs it in a separate sanitizer profile.

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
