// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::expected<T,E> is a value-or-error type with monadic and_then / or_else / transform.
// reference: https://en.cppreference.com/w/cpp/utility/expected

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
    auto ok = parse_positive("42");
    assert(ok.has_value());
    assert(*ok == 42);

    auto bad = parse_positive("oops");
    assert(!bad.has_value());
    assert(bad.error() == ParseError::NotANumber);

    // Monadic chaining.
    auto doubled = parse_positive("21")
                       .transform([](int n) { return n * 2; });
    assert(*doubled == 42);

    auto fallback = parse_positive("")
                        .or_else([](ParseError) -> std::expected<int, ParseError> {
                            return 0;
                        });
    assert(*fallback == 0);

    auto chained = parse_positive("10")
                       .and_then([](int n) -> std::expected<int, ParseError> {
                           return n * n;
                       });
    assert(*chained == 100);

    // value_or for the happy default.
    assert(parse_positive("oops").value_or(-1) == -1);
    return 0;
}
