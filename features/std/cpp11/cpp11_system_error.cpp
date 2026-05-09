// description: <system_error> standardises error reporting -- error_code is a (value, category) pair; std::system_error is the matching exception. Used by <filesystem>, <future>, networking, etc.
// reference: https://en.cppreference.com/w/cpp/header/system_error

#include "support/demo.hpp"
#include <cerrno>
#include <string>
#include <system_error>

// Define a custom error_category. Real code uses this for protocol-level errors,
// driver return codes, etc. -- anything that has a small set of named failures.
enum class parse_error { ok = 0, empty_input, bad_digit, overflow };

class parse_error_category : public std::error_category {
public:
    const char* name() const noexcept override { return "parse"; }
    std::string message(int ev) const override {
        switch (static_cast<parse_error>(ev)) {
            case parse_error::ok:          return "ok";
            case parse_error::empty_input: return "empty input";
            case parse_error::bad_digit:   return "non-digit character";
            case parse_error::overflow:    return "value out of range";
        }
        return "unknown";
    }
};

const std::error_category& parse_category() {
    static const parse_error_category instance;
    return instance;
}

std::error_code make_error_code(parse_error e) {
    return {static_cast<int>(e), parse_category()};
}

int main() {
    demo::title("C++11 system_error");

    // generic_category() / system_category() come from the standard.
    std::error_code ec(EINVAL, std::generic_category());
    demo::value("ec.value", ec.value());
    demo::text("ec.message", ec.message());
    demo::text("ec.category", ec.category().name());
    DEMO_ASSERT(static_cast<bool>(ec));   // any non-zero code is "truthy"

    // Custom category round-trip.
    auto custom = make_error_code(parse_error::bad_digit);
    demo::text("custom.message", custom.message());
    DEMO_ASSERT(custom.category().name() == std::string("parse"));

    // std::system_error wraps an error_code as a throwable exception.
    try {
        throw std::system_error(make_error_code(parse_error::overflow), "stoi");
    } catch (const std::system_error& e) {
        demo::text("caught", e.what());
    }
    return 0;
}
