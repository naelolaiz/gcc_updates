// description: std::unreachable() marks code paths that cannot be reached -- the optimizer assumes UB if they ever are.
// reference: https://en.cppreference.com/w/cpp/utility/unreachable

#include "support/demo.hpp"
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
    demo::title("C++23 unreachable");
    DEMO_ASSERT(mode_to_flags(Mode::Read)   == 1);
    DEMO_ASSERT(mode_to_flags(Mode::Write)  == 2);
    DEMO_ASSERT(mode_to_flags(Mode::Append) == 4);
    return 0;
}
