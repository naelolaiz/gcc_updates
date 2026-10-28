// description: std::optional gained monadic ops (and_then / transform / or_else) -- chain fallible computations cleanly.
// reference: https://en.cppreference.com/w/cpp/utility/optional
// why: Fallible steps can compose into a single expression pipeline without repetitive 'if (opt)' unwrap boilerplate.
// before: Every step checked 'if (!opt) return std::nullopt;' before computing the next optional value.
// pitfall: 'transform' wraps its callable's return value in std::optional, whereas 'and_then' requires the callable itself to return a std::optional.

#include "support/demo.hpp"
#include <cassert>
#include <optional>

std::optional<int> parse(int n) {
    return n > 0 ? std::optional{n} : std::nullopt;
}

std::optional<int> reciprocal_int(int n) {
    return n != 0 ? std::optional{1000 / n} : std::nullopt;
}

int main() {
    demo::title("C++23 optional monadic");
    auto pipeline = parse(5)
                        .and_then(reciprocal_int)
                        .transform([](int x) { return x * 2; });
    DEMO_ASSERT(*pipeline == 400);   // 1000/5 = 200; *2 = 400

    // Short-circuit on failure: parse returns nullopt, downstream skipped.
    auto fail = parse(-1)
                    .and_then(reciprocal_int)
                    .transform([](int x) { return x * 2; });
    DEMO_ASSERT(!fail.has_value());

    // or_else for fallback.
    auto with_default = parse(-1)
                            .or_else([] { return std::optional{42}; });
    DEMO_ASSERT(*with_default == 42);
    return 0;
}
