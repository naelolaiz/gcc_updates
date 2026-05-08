// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'nullptr' has its own type std::nullptr_t and isn't ambiguous between integer and pointer overloads.
// reference: https://en.cppreference.com/w/cpp/language/nullptr

#include "support/demo.hpp"
#include <cassert>
#include <cstddef>
#include <type_traits>

void overload(int)    { /* hit only with NULL on some impls */ }
void overload(int*) { }

bool got_pointer = false;
void calls_int(int)     { got_pointer = false; }
void calls_int(int*)    { got_pointer = true; }

int main() {
    demo::title("C++11 nullptr");
    int* p = nullptr;
    DEMO_ASSERT(p == nullptr);

    static_assert(std::is_same<decltype(nullptr), std::nullptr_t>::value, "");

    // nullptr unambiguously selects the pointer overload.
    calls_int(nullptr);
    DEMO_ASSERT(got_pointer);

    // ... whereas '0' or 'NULL' would select the int overload (legacy footgun).
    calls_int(0);
    DEMO_ASSERT(!got_pointer);
    return 0;
}
