// gcc-test: std=c++17 min-gcc=13 topic=stl experimental=false
// description: std::variant<Ts...> is a type-safe tagged union; std::visit dispatches on the active alternative.
// reference: https://en.cppreference.com/w/cpp/utility/variant

#include <cassert>
#include <string>
#include <variant>

// Helper: overload set built from N lambdas (a common pattern).
template <typename... Fs> struct overloaded : Fs... { using Fs::operator()...; };
// CTAD guide -- C++17 also added user-defined deduction guides.
template <typename... Fs> overloaded(Fs...) -> overloaded<Fs...>;

int main() {
    std::variant<int, std::string, double> v = 42;
    assert(v.index() == 0);
    assert(std::get<int>(v) == 42);
    assert(std::holds_alternative<int>(v));

    v = std::string("hello");
    assert(v.index() == 1);
    assert(std::get<std::string>(v) == "hello");

    v = 3.14;
    assert(std::holds_alternative<double>(v));

    // get_if returns a pointer or nullptr.
    if (auto* p = std::get_if<double>(&v)) {
        assert(*p == 3.14);
    }

    // visit + overloaded for type-driven dispatch.
    int summary = std::visit(overloaded{
        [](int i)               { return i * 2; },
        [](const std::string&)  { return -1; },
        [](double d)            { return static_cast<int>(d * 100); },
    }, v);
    assert(summary == 314);
    return 0;
}
