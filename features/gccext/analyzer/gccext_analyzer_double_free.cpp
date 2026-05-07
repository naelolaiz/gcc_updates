// gcc-test: std=c++17 min-gcc=16 topic=gcc-analyzer experimental=false requires-analyzer=true
// description: -fanalyzer flags a double-delete that only fires on a control-flow branch the runtime might never hit; runtime sanitizers can miss this if the test inputs don't reach that path.
// reference: https://gcc.gnu.org/onlinedocs/gcc/Static-Analyzer-Options.html

// Compiled under -fanalyzer only; binary is never executed.
// Expect a -Wanalyzer-double-free diagnostic on the argc>1 branch.

#include "support/demo.hpp"
#include <new>

int main(int argc, char**) {
    demo::title("GCC extension analyzer double free");
    int* p = new int(0);
    delete p;
    if (argc > 1) {
        delete p;   // analyzer: 'p' was already freed on the line above
    }
    return 0;
}
