// description: std::expected<T,E> is a value-or-error type with monadic and_then / or_else / transform.
// reference: https://en.cppreference.com/w/cpp/utility/expected
// why: Recoverable failures can carry a typed error without exceptions or output parameters.
// before: APIs used sentinel values, bool-plus-output pairs, or a custom result union.
// pitfall: value() on an error throws; monadic operations preserve or transform the error path explicitly.

#include "support/demo.hpp"
#include <cassert>
#include <expected>
#include <string>
#include <string_view>

enum class ParseError { Empty, NotANumber };

std::expected<int, ParseError> parse_positive(std::string_view s) {
    if (s.empty()) return std::unexpected{ParseError::Empty};
    int n = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::unexpected{ParseError::NotANumber};
        n = n * 10 + (c - '0');
    }
    return n;
}

int main() {
    demo::title("C++23 expected");
    auto ok = parse_positive("42");
    DEMO_ASSERT(ok.has_value());
    DEMO_ASSERT(*ok == 42);

    auto bad = parse_positive("oops");
    DEMO_ASSERT(!bad.has_value());
    DEMO_ASSERT(bad.error() == ParseError::NotANumber);

    // Monadic chaining.
    auto doubled = parse_positive("21")
                       .transform([](int n) { return n * 2; });
    DEMO_ASSERT(*doubled == 42);

    auto fallback = parse_positive("")
                        .or_else([](ParseError) -> std::expected<int, ParseError> {
                            return 0;
                        });
    DEMO_ASSERT(*fallback == 0);

    auto chained = parse_positive("10")
                       .and_then([](int n) -> std::expected<int, ParseError> {
                           return n * n;
                       });
    DEMO_ASSERT(*chained == 100);

    // value_or for the happy default.
    DEMO_ASSERT(parse_positive("oops").value_or(-1) == -1);
    return 0;
}
