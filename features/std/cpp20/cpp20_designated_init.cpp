// gcc-test: std=c++20 min-gcc=14 topic=language experimental=false
// description: Designated initializers let you initialise aggregate members by name; order must match declaration.
// reference: https://en.cppreference.com/w/cpp/language/aggregate_initialization

#include <cassert>
#include <string>

struct Config {
    std::string host;
    int port;
    bool tls;
    int timeout_ms;
};

int main() {
    Config c{
        .host = "example.com",
        .port = 443,
        .tls  = true,
        .timeout_ms = 5000,
    };
    assert(c.host == "example.com");
    assert(c.port == 443);
    assert(c.tls);
    assert(c.timeout_ms == 5000);

    // Skipping members is allowed; they get value-initialized.
    Config minimal{.host = "localhost", .port = 80};
    assert(!minimal.tls);
    assert(minimal.timeout_ms == 0);

    return 0;
}
