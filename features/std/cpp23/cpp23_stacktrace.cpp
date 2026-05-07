// gcc-test: std=c++23 min-gcc=14 min-libstdcxx=14 topic=stl experimental=false extra-flags=-lstdc++exp
// description: std::stacktrace captures and pretty-prints the current call stack; symbol names depend on debuginfo.
// reference: https://en.cppreference.com/w/cpp/utility/basic_stacktrace
// note: per cppreference, std::stacktrace ships in libstdc++exp on libstdc++ 14+;
// link with -lstdc++exp.

#include "support/demo.hpp"
#include <cassert>
#include <stacktrace>
#include <string>

[[gnu::noinline]] std::stacktrace inner() {
    return std::stacktrace::current();
}

[[gnu::noinline]] std::stacktrace outer() {
    return inner();
}

int main() {
    demo::title("C++23 stacktrace");
    auto trace = outer();
    DEMO_ASSERT(trace.size() >= 1);

    std::string text = std::to_string(trace);
    // We don't assert specific symbols (debuginfo dependent), only that the
    // trace produced *some* non-empty textual representation.
    DEMO_ASSERT(!text.empty());
    return 0;
}
