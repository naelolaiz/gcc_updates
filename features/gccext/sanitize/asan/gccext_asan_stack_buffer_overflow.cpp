// gcc-test: std=c++17 min-gcc=13 topic=gcc-sanitize experimental=false requires-sanitizer=address expect-exit=1
// description: ASan demo: writing past the end of a stack array. Runs only under -fsanitize=address; ASan reports 'stack-buffer-overflow'.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html

#include "support/demo.hpp"

[[gnu::noinline]] int touch(int* p, int i) { return p[i] = 99; }


int main() {
    demo::title("GCC extension asan stack buffer overflow");
    int arr[4]{};
    return touch(arr, 4);   // ASan: stack-buffer-overflow
}
