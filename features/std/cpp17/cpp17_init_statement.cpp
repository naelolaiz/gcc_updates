// gcc-test: std=c++17 min-gcc=13 topic=language experimental=false
// description: if/switch can carry an init-statement: 'if (auto x = expr; cond)'. Locals stay scoped.
// reference: https://en.cppreference.com/w/cpp/language/if

#include <cassert>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> m{{"a", 1}, {"b", 2}};

    // Init-statement keeps the iterator local to the if/else block.
    if (auto it = m.find("a"); it != m.end()) {
        assert(it->second == 1);
    } else {
        assert(false);
    }

    // The iterator is no longer in scope here.
    if (auto it = m.find("missing"); it == m.end()) {
        // OK
    } else {
        assert(false);
    }

    // switch with init.
    int total = 0;
    switch (int v = 3 * 7; v % 4) {
        case 0: total += 100; break;
        case 1: total += v;   break;          // 21 % 4 == 1 -> total += 21
        default: total += -1; break;
    }
    assert(total == 21);
    return 0;
}
