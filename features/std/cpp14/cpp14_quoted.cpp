// description: std::quoted(s) streams a string wrapped in quotes with embedded quotes escaped, and reads it back the same way -- round-trip strings containing spaces through any iostream.
// reference: https://en.cppreference.com/w/cpp/io/manip/quoted

#include "support/demo.hpp"
#include <iomanip>
#include <sstream>
#include <string>

int main() {
    demo::title("C++14 std::quoted");

    const std::string original = "a \"quoted\" path with spaces";

    std::stringstream ss;
    ss << std::quoted(original);
    demo::text("on the wire", ss.str());

    std::string restored;
    ss >> std::quoted(restored);
    DEMO_ASSERT(restored == original);  // spaces and quotes survive intact

    // Without quoted, operator>> stops at the first space.
    std::stringstream plain("hello world");
    std::string word;
    plain >> word;
    DEMO_ASSERT(word == "hello");

    // Delimiter and escape characters are the optional 2nd/3rd arguments.
    std::stringstream custom;
    custom << std::quoted("select 'x'", '\'');
    demo::text("single-quote delim", custom.str());
    DEMO_ASSERT(custom.str() == "'select \\'x\\''");
    return 0;
}
