// gcc-test: std=c++23 min-gcc=14 min-libstdcxx=14 topic=stl experimental=false extra-flags=-lstdc++exp
// description: std::stacktrace captures and pretty-prints the current call stack; symbol names depend on debuginfo.
// reference: https://en.cppreference.com/w/cpp/utility/basic_stacktrace
// note: per cppreference, std::stacktrace ships in libstdc++exp on libstdc++ 14+;
// link with -lstdc++exp.

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
    auto trace = outer();
    assert(trace.size() >= 1);

    std::string text = std::to_string(trace);
    // We don't assert specific symbols (debuginfo dependent), only that the
    // trace produced *some* non-empty textual representation.
    assert(!text.empty());
    return 0;
}
