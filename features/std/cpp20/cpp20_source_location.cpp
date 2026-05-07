// gcc-test: std=c++20 min-gcc=14 topic=stl experimental=false
// description: std::source_location replaces __FILE__/__LINE__/__func__ macros with a real value type.
// reference: https://en.cppreference.com/w/cpp/utility/source_location

#include <cassert>
#include <source_location>
#include <string_view>

struct Trace {
    std::string_view file;
    unsigned line;
    std::string_view function;
};

Trace where(std::source_location loc = std::source_location::current()) {
    return {loc.file_name(), loc.line(), loc.function_name()};
}

int main() {
    auto here = where(); // captures the call site, not where() itself
    auto file_sv = std::string_view(here.file);
    assert(file_sv.find("cpp20_source_location.cpp") != std::string_view::npos);
    assert(here.line > 0);
    assert(std::string_view(here.function).find("main") != std::string_view::npos);
    return 0;
}
