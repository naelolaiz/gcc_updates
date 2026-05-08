// description: [[noreturn]] tells the compiler a function never returns -- enables better diagnostics and dead-code elimination.
// reference: https://en.cppreference.com/w/cpp/language/attributes/noreturn

#include "support/demo.hpp"
#include <cassert>
#include <stdexcept>

[[noreturn]] void fail(const char* msg) { throw std::runtime_error(msg); }

int classify(int x) {
    if (x < 0) fail("negative");
    if (x == 0) return 0;
    return 1;
    // No 'return after fail()' needed; compiler knows fail() never returns.
}

int main() {
    demo::title("C++11 attribute noreturn");
    DEMO_ASSERT(classify(0) == 0);
    DEMO_ASSERT(classify(5) == 1);

    bool caught = false;
    try { (void)classify(-1); }
    catch (const std::runtime_error&) { caught = true; }
    DEMO_ASSERT(caught);
    return 0;
}
