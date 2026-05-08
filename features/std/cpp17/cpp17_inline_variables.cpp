// description: 'inline' variables can be defined in a header without the ODR violation; perfect for header-only constants.
// reference: https://en.cppreference.com/w/cpp/language/inline

#include "support/demo.hpp"
#include <cassert>
#include <string>

// Definable in a header without violating ODR.
inline const std::string kAppName = "gcc_updates";
inline constexpr int      kMagic   = 42;

struct Config {
    // inline static member = no separate definition needed in a .cpp.
    inline static int instances = 0;
    Config() { ++instances; }
};

int main() {
    demo::title("C++17 inline variables");
    DEMO_ASSERT(kAppName == "gcc_updates");
    static_assert(kMagic == 42);

    Config a, b;
    DEMO_ASSERT(Config::instances == 2);
    return 0;
}
