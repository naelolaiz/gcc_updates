// description: std::any is a heterogenous box that holds any copyable type; std::any_cast extracts.
// reference: https://en.cppreference.com/w/cpp/utility/any

#include "support/demo.hpp"
#include <any>
#include <cassert>
#include <string>

int main() {
    demo::title("C++17 any");
    std::any a = 42;
    DEMO_ASSERT(a.has_value());
    DEMO_ASSERT(a.type() == typeid(int));
    DEMO_ASSERT(std::any_cast<int>(a) == 42);

    a = std::string("hello");
    DEMO_ASSERT(a.type() == typeid(std::string));
    DEMO_ASSERT(std::any_cast<std::string>(a) == "hello");

    // any_cast on a wrong type throws.
    bool threw = false;
    try { (void)std::any_cast<int>(a); } catch (const std::bad_any_cast&) { threw = true; }
    DEMO_ASSERT(threw);

    // Pointer form returns nullptr on mismatch instead of throwing.
    DEMO_ASSERT(std::any_cast<int>(&a) == nullptr);
    DEMO_ASSERT(std::any_cast<std::string>(&a) != nullptr);

    a.reset();
    DEMO_ASSERT(!a.has_value());
    return 0;
}
