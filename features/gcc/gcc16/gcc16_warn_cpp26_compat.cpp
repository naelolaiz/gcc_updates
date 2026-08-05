// description: GCC 16 introduced -Wc++26-compat: identifiers that become keywords in C++26 (like contract_assert) are flagged when compiling as an older standard. Built as C++23 with -Werror=c++26-compat; CTest asserts the diagnostic fires.
// reference: https://gcc.gnu.org/gcc-16/changes.html

// Deliberately broken -- this file must NOT compile; the test expects the
// diagnostic text (see CMakeLists.txt).
// 'contract_assert' is a plain identifier in C++23 and a keyword in C++26.
int contract_assert = 0;

int main() { return contract_assert; }
