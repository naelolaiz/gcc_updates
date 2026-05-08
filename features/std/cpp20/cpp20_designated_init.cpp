// description: Designated initializers let you initialise aggregate members by name; order must match declaration.
// reference: https://en.cppreference.com/w/cpp/language/aggregate_initialization

#include "support/demo.hpp"
#include <cassert>
#include <string>

struct Config {
    std::string host;
    int port;
    bool tls = false;
    int timeout_ms = 0;
};

int main() {
    demo::title("C++20 designated init");
    Config c{
        .host = "example.com",
        .port = 443,
        .tls  = true,
        .timeout_ms = 5000,
    };
    DEMO_ASSERT(c.host == "example.com");
    DEMO_ASSERT(c.port == 443);
    DEMO_ASSERT(c.tls);
    DEMO_ASSERT(c.timeout_ms == 5000);

    // Skipping members is allowed; they fall back to the default member
    // initializer (or value-init if the struct doesn't supply one).
    Config minimal{.host = "localhost", .port = 80};
    DEMO_ASSERT(!minimal.tls);
    DEMO_ASSERT(minimal.timeout_ms == 0);

    return 0;
}
