// description: constinit forces a static/thread_local variable to be initialised at compile time (killing static-init-order surprises) while staying mutable at runtime -- unlike constexpr, which also makes it const.
// reference: https://en.cppreference.com/w/cpp/language/constinit

#include "support/demo.hpp"
#include <atomic>

constexpr int compute_initial() { return 6 * 7; }

// Guaranteed static (compile-time) initialisation; still mutable afterwards.
constinit int counter = compute_initial();
constinit thread_local int per_thread = 5;
constinit std::atomic<int> flag{1};

// constinit int bad = std::rand();  // would not compile: not a constant expression

int main() {
    demo::title("C++20 constinit");

    DEMO_ASSERT(counter == 42);
    counter += 1;  // mutable -- a constexpr variable would reject this
    DEMO_ASSERT(counter == 43);

    DEMO_ASSERT(per_thread == 5);
    DEMO_ASSERT(flag.load() == 1);
    demo::value("counter", counter);
    return 0;
}
