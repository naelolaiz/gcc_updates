// description: ASan demo: read after free. Runs only under -fsanitize=address; ASan reports 'heap-use-after-free' and aborts.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
// why: Freed heap storage may retain plausible data while no longer belonging to the program.
// before: The dangling read depended on allocator reuse or a difficult debugger trace.
// pitfall: This file intentionally has undefined behavior and is valid only in the ASan lane.

#include "support/demo.hpp"
#include <cstdlib>

int main() {
    demo::title("GCC extension asan heap use after free");
    auto* p = static_cast<int*>(std::malloc(sizeof(int) * 4));
    p[0] = 42;
    std::free(p);
    return p[0];   // ASan: heap-use-after-free
}
