// gcc-test: std=c++17 min-gcc=13 topic=gcc-pragmas experimental=false
// description: '#pragma GCC diagnostic push/pop/ignored' silences a warning locally without -Wno-* polluting the rest of the file.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html

#include <cassert>

int unused_in_default_build() { return 0; }

int main() {
    // Locally suppress -Wunused-variable for one block.
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-variable"
    int intentionally_unused = 42;
    #pragma GCC diagnostic pop

    (void)intentionally_unused;
    assert(unused_in_default_build() == 0);
    return 0;
}
