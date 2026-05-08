// description: __attribute__((cleanup(fn))) calls fn(&var) when var goes out of scope -- C-style RAII; redundant in C++ but works there too.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html

#include "support/demo.hpp"
#include <cassert>

int destroy_count = 0;

static void close_handle(int* h) {
    if (*h != -1) {
        ++destroy_count;
        *h = -1;
    }
}

int main() {
    demo::title("GCC extension attribute cleanup");
    {
        int handle __attribute__((cleanup(close_handle))) = 42;
        DEMO_ASSERT(handle == 42);
        // close_handle(&handle) fires here.
    }
    DEMO_ASSERT(destroy_count == 1);
    return 0;
}
