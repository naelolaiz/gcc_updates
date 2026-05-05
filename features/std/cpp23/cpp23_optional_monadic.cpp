// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false
// description: std::optional gained monadic ops (and_then / transform / or_else) -- chain fallible computations cleanly.
// reference: https://en.cppreference.com/w/cpp/utility/optional

#include <cassert>
#include <optional>

std::optional<int> parse(int n) {
    return n > 0 ? std::optional{n} : std::nullopt;
}

std::optional<int> reciprocal_int(int n) {
    return n != 0 ? std::optional{1000 / n} : std::nullopt;
}

int main() {
    auto pipeline = parse(5)
                        .and_then(reciprocal_int)
                        .transform([](int x) { return x * 2; });
    assert(*pipeline == 400);   // 1000/5 = 200; *2 = 400

    // Short-circuit on failure: parse returns nullopt, downstream skipped.
    auto fail = parse(-1)
                    .and_then(reciprocal_int)
                    .transform([](int x) { return x * 2; });
    assert(!fail.has_value());

    // or_else for fallback.
    auto with_default = parse(-1)
                            .or_else([] { return std::optional{42}; });
    assert(*with_default == 42);
    return 0;
}
