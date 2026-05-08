// description: std::out_ptr / std::inout_ptr adapt smart pointers to legacy 'T**' APIs without manual reset() dancing.
// reference: https://en.cppreference.com/w/cpp/memory/out_ptr_t

#include "support/demo.hpp"
#include <cassert>
#include <cstdlib>
#include <memory>

// Pretend C-style API that allocates via malloc and returns through T**.
int legacy_create(int** out) {
    *out = static_cast<int*>(std::malloc(sizeof(int)));
    if (!*out) return -1;
    **out = 42;
    return 0;
}

int main() {
    demo::title("C++23 out ptr");
    std::unique_ptr<int, decltype([](int* p){ std::free(p); })> p;

    // out_ptr adapts unique_ptr's internal pointer slot to be writable as int**.
    int rc = legacy_create(std::out_ptr(p));
    DEMO_ASSERT(rc == 0);
    DEMO_ASSERT(p);
    DEMO_ASSERT(*p == 42);
    // p destructor frees the malloc'd block via the deleter.
    return 0;
}
