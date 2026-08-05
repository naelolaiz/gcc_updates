// description: C++26 gives std::span bounds-checked element access: span.at(i) throws std::out_of_range like vector::at -- opt-in safety for the otherwise unchecked view.
// reference: https://en.cppreference.com/w/cpp/container/span/at

#include "support/demo.hpp"
#include <span>
#include <stdexcept>

int main() {
    demo::title("C++26 span::at");

    int raw[4] = {10, 20, 30, 40};
    std::span<int> s(raw);

    DEMO_ASSERT(s.at(2) == 30);  // in range: identical to s[2]
    s.at(1) = 25;                // returns a reference, so writes go through
    DEMO_ASSERT(raw[1] == 25);

    bool threw = false;
    try {
        (void)s.at(4);  // one past the end -- s[4] would be silent UB
    } catch (const std::out_of_range&) {
        threw = true;
    }
    DEMO_ASSERT(threw);
    return 0;
}
