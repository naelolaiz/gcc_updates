// description: std::variant<Ts...> is a type-safe tagged union; std::visit dispatches on the active alternative.
// reference: https://en.cppreference.com/w/cpp/utility/variant
// why: One value can hold one of several known types while preserving which type is active.
// before: Unions required a separate tag and manual construction, destruction, and dispatch.
// pitfall: std::get<T> throws when T is not active; use holds_alternative, get_if, or visit.

#include "support/demo.hpp"
#include <cassert>
#include <string>
#include <variant>

// Helper: overload set built from N lambdas (a common pattern).
template <typename... Fs> struct overloaded : Fs... { using Fs::operator()...; };
// CTAD guide -- C++17 also added user-defined deduction guides.
template <typename... Fs> overloaded(Fs...) -> overloaded<Fs...>;

int main() {
    demo::title("C++17 variant");
    std::variant<int, std::string, double> v = 42;
    DEMO_ASSERT(v.index() == 0);
    DEMO_ASSERT(std::get<int>(v) == 42);
    DEMO_ASSERT(std::holds_alternative<int>(v));

    v = std::string("hello");
    DEMO_ASSERT(v.index() == 1);
    DEMO_ASSERT(std::get<std::string>(v) == "hello");

    v = 3.14;
    DEMO_ASSERT(std::holds_alternative<double>(v));

    // get_if returns a pointer or nullptr.
    if (auto* p = std::get_if<double>(&v)) {
        DEMO_ASSERT(*p == 3.14);
    }

    // visit + overloaded for type-driven dispatch.
    int summary = std::visit(overloaded{
        [](int i)               { return i * 2; },
        [](const std::string&)  { return -1; },
        [](double d)            { return static_cast<int>(d * 100); },
    }, v);
    DEMO_ASSERT(summary == 314);
    return 0;
}
