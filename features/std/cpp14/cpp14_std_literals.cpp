// description: Standard user-defined literals: "text"s builds a std::string, 1500ms / 1.5s build std::chrono durations, 4.0i builds a std::complex -- typed constants with no constructor noise.
// reference: https://en.cppreference.com/w/cpp/symbol_index/literals

#include "support/demo.hpp"
#include <chrono>
#include <cmath>
#include <complex>
#include <string>
#include <type_traits>

int main() {
    demo::title("C++14 standard literals");

    using namespace std::literals;  // s, ms, i, ... in one line

    // "..."s is a real std::string, not a const char* -- so + and .size() work
    // immediately, and embedded '\0' bytes survive.
    auto greeting = "hello"s + " world";
    static_assert(std::is_same<decltype(greeting), std::string>::value, "");
    DEMO_ASSERT(greeting.size() == 11);
    DEMO_ASSERT("a\0b"s.size() == 3);

    // Chrono literals make durations readable and unit-safe; mixed-unit
    // comparison converts through std::common_type.
    auto timeout = 1500ms;
    DEMO_ASSERT(timeout == 1.5s);
    DEMO_ASSERT(std::chrono::duration_cast<std::chrono::seconds>(timeout).count() == 1);
    demo::value("timeout.count()", timeout.count());

    // Complex literals: i / if / il for the imaginary part.
    auto z = 3.0 + 4.0i;
    DEMO_ASSERT(std::abs(z) == 5.0);
    demo::value("abs(3+4i)", std::abs(z));
    return 0;
}
