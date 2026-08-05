// description: std::text_encoding identifies character encodings via the IANA registry: text_encoding::literal() reports the encoding of the string literals baked into this binary, environment() the locale's.
// reference: https://en.cppreference.com/w/cpp/text/text_encoding

#include "support/demo.hpp"
#include <text_encoding>

int main() {
    demo::title("C++26 text_encoding");

    // The encoding of this translation unit's literals. GCC's default
    // -fexec-charset is UTF-8, and this is now queryable instead of assumed.
    std::text_encoding lit = std::text_encoding::literal();
    demo::text("literal encoding", lit.name());
    DEMO_ASSERT(lit.mib() == std::text_encoding::id::UTF8);

    // The runtime environment's encoding is locale-dependent (plain "C"
    // locale in CI reports ASCII), so print it rather than assert on it.
    std::text_encoding env = std::text_encoding::environment();
    demo::text("environment encoding", env.name());
    return 0;
}
