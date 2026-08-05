// description: GCC 15 introduced -Wdeprecated-literal-operator: C++23 deprecated the space between "" and the suffix when declaring a literal operator. Built with -Werror=deprecated-literal-operator; CTest asserts the diagnostic fires.
// reference: https://gcc.gnu.org/onlinedocs/gcc-15.1.0/gcc/Warning-Options.html

// Deliberately broken -- this file must NOT compile; the test expects the
// diagnostic text (see CMakeLists.txt). Modern spelling: operator""_kb.
unsigned long long operator "" _kb(unsigned long long v) { return v * 1024ull; }

int main() { return 2_kb == 2048 ? 0 : 1; }
