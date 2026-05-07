// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: [[noreturn]] tells the compiler a function never returns -- enables better diagnostics and dead-code elimination.
// reference: https://en.cppreference.com/w/cpp/language/attributes/noreturn

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
    assert(classify(0) == 0);
    assert(classify(5) == 1);

    bool caught = false;
    try { (void)classify(-1); }
    catch (const std::runtime_error&) { caught = true; }
    assert(caught);
    return 0;
}
