// description: Class types can now be non-type template parameters (NTTPs); enables compile-time string templates.
// reference: https://en.cppreference.com/w/cpp/language/template_parameters#Non-type_template_parameter

#include "support/demo.hpp"
#include <algorithm>
#include <cassert>
#include <string_view>
#include <type_traits>

// A literal class -- structural type usable as NTTP.
template <std::size_t N>
struct FixedString {
    char data[N]{};
    constexpr FixedString(const char (&s)[N]) {
        std::copy_n(s, N, data);
    }
    constexpr std::string_view view() const { return {data, N - 1}; }
    constexpr auto operator<=>(const FixedString&) const = default;
};

// CTAD guide so users write FixedString{"abc"} not FixedString<4>{"abc"}.
template <std::size_t N>
FixedString(const char (&)[N]) -> FixedString<N>;

template <FixedString Tag>
struct TaggedCounter {
    int hits = 0;
    static constexpr std::string_view tag() { return Tag.view(); }
};

int main() {
    demo::title("C++20 nttp class");
    TaggedCounter<"hello"> a;
    TaggedCounter<"world"> b;

    static_assert(a.tag() == "hello");
    static_assert(b.tag() == "world");
    static_assert(!std::is_same_v<decltype(a), decltype(b)>);

    a.hits++;
    b.hits += 5;
    DEMO_ASSERT(a.hits == 1);
    DEMO_ASSERT(b.hits == 5);
    return 0;
}
