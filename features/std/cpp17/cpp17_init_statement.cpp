// description: if/switch can carry an init-statement: 'if (auto x = expr; cond)'. Locals stay scoped.
// reference: https://en.cppreference.com/w/cpp/language/if

#include "support/demo.hpp"
#include <cassert>
#include <map>
#include <string>

int main() {
    demo::title("C++17 init statement");
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};

    // Init-statement keeps the iterator local to the if/else block.
    if (auto it = m.find("a"); it != m.end()) {
        DEMO_ASSERT(it->second == 1);
    } else {
        DEMO_ASSERT(false);
    }

    // The iterator is no longer in scope here.
    if (auto it = m.find("missing"); it == m.end()) {
        // OK
    } else {
        DEMO_ASSERT(false);
    }

    // switch with init.
    int total = 0;
    switch (int v = 3 * 7; v % 4) {
        case 0: total += 100; break;
        case 1: total += v;   break;          // 21 % 4 == 1 -> total += 21
        default: total += -1; break;
    }
    DEMO_ASSERT(total == 21);
    return 0;
}
