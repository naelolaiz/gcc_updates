// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::unreachable() marks code paths that cannot be reached -- the optimizer assumes UB if they ever are.
// reference: https://en.cppreference.com/w/cpp/utility/unreachable

#include <cassert>
#include <utility>

enum class Mode { Read, Write, Append };

int mode_to_flags(Mode m) {
    switch (m) {
        case Mode::Read:   return 1;
        case Mode::Write:  return 2;
        case Mode::Append: return 4;
    }
    // Reaching here would mean an invalid Mode value -- compiler-assistance
    // hint that the switch is exhaustive.
    std::unreachable();
}

int main() {
    assert(mode_to_flags(Mode::Read)   == 1);
    assert(mode_to_flags(Mode::Write)  == 2);
    assert(mode_to_flags(Mode::Append) == 4);
    return 0;
}
