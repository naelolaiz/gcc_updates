// description: GCC 13 introduced -Wdangling-reference: binding a reference to a call that derives its result from a temporary argument is flagged at compile time. Built with -Werror=dangling-reference; CTest asserts the diagnostic fires.
// reference: https://gcc.gnu.org/onlinedocs/gcc-13.1.0/gcc/Warning-Options.html

// Deliberately broken -- this file must NOT compile; the test expects the
// diagnostic text (see CMakeLists.txt).
const int& smaller(const int& a, const int& b) { return a < b ? a : b; }

int main() {
    // Both temporaries die at the end of the full expression; r dangles.
    const int& r = smaller(1, 2);
    return r;
}
