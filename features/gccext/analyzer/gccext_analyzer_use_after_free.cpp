// gcc-test: std=c++17 min-gcc=16 topic=gcc-analyzer experimental=false requires-analyzer=true
// description: -fanalyzer warns at compile time about use-after-delete on a path through main(). GCC 16 is the first version where this works on C++ (NRVO + initial exception support).
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html

// This file is compiled-only under the analyzer job; the binary is never run.
// Expect a -Wanalyzer-use-after-free diagnostic in the build log.

#include <new>

[[gnu::noinline]] int read_value(int* p) { return *p; }

int main() {
    int* p = new int(42);
    delete p;
    return read_value(p);   // analyzer: dereferences memory deleted on the line above
}
