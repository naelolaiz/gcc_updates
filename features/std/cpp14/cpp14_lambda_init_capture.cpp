// description: Lambda init-captures: '[x = expr]' creates a closure member computed at capture time -- enabling move-capture ('[p = std::move(p)]'), which C++11 lambdas could not express.
// reference: https://en.cppreference.com/w/cpp/language/lambda

#include "support/demo.hpp"
#include <memory>
#include <string>
#include <utility>

int main() {
    demo::title("C++14 lambda init capture");

    // Move a unique_ptr INTO the closure -- impossible in C++11, whose
    // captures could only copy or reference an existing variable.
    auto owner = [p = std::make_unique<int>(42)] { return *p; };
    DEMO_ASSERT(owner() == 42);

    // The init expression is arbitrary; the capture needn't name an
    // outer variable at all.
    int base = 10;
    auto plus_squared = [sq = base * base](int x) { return x + sq; };
    DEMO_ASSERT(plus_squared(5) == 105);

    // Combined with 'mutable', the closure carries evolving private state.
    auto counter = [n = 0]() mutable { return ++n; };
    counter();
    counter();
    DEMO_ASSERT(counter() == 3);

    // Move an expensive-to-copy object in; the closure owns its own copy.
    std::string payload = "expensive payload";
    auto shout = [msg = std::move(payload)] { return msg + "!"; };
    DEMO_ASSERT(shout() == "expensive payload!");
    demo::text("shout()", shout());
    return 0;
}
