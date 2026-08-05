// description: GCC 14 introduced -Wcalloc-transposed-args: calloc(sizeof(T), n) has the arguments backwards (element count first, element size second). Built with -Werror=calloc-transposed-args; CTest asserts the diagnostic fires.
// reference: https://gcc.gnu.org/onlinedocs/gcc-14.1.0/gcc/Warning-Options.html

// Deliberately broken -- this file must NOT compile; the test expects the
// diagnostic text (see CMakeLists.txt).
#include <cstdlib>

int main() {
    // Should be calloc(8, sizeof(int)): number of elements comes first.
    int* p = static_cast<int*>(calloc(sizeof(int), 8));
    free(p);
    return 0;
}
