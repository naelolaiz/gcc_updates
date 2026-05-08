// description: UBSan demo: dereferencing a null pointer. Runs only under -fsanitize=undefined; reports the null deref and aborts.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include "support/demo.hpp"

[[gnu::noinline]] int read_through(int* p) { return *p; }   // UB if p == nullptr


int main() {
    demo::title("GCC extension ubsan null deref");
    int* p = nullptr;
    return read_through(p);   // UBSan: load of null pointer
}
