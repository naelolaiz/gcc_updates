// description: A constrained overload rejects a non-integral argument at selection time and produces a constraints-not-satisfied diagnostic.
// reference: https://en.cppreference.com/w/cpp/language/constraints
// why: Callers should see a failed requirement at the interface, not an error deep in a template body.
// before: SFINAE traits and enable_if encoded the same restriction less directly.
// pitfall: A concept states syntactic and semantic requirements; it does not convert the argument.

#include <concepts>

template <std::integral T>
constexpr T twice(T value) {
    return value + value;
}

int main() {
    return twice("not an integer");
}
