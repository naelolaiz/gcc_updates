// description: std::string::resize_and_overwrite gives you an uninitialized buffer of N bytes, then trusts your callback to size it down.
// reference: https://en.cppreference.com/w/cpp/string/basic_string/resize_and_overwrite

#include "support/demo.hpp"
#include <cassert>
#include <cstring>
#include <string>

int main() {
    demo::title("C++23 resize and overwrite");
    std::string s;
    // Reserve up to 64 bytes; let the callback decide how many were actually used.
    s.resize_and_overwrite(64, [](char* buf, [[maybe_unused]] std::size_t cap) -> std::size_t {
        const char* src = "hello, world";
        std::memcpy(buf, src, 12);
        return 12;
    });
    DEMO_ASSERT(s.size() == 12);
    DEMO_ASSERT(s == "hello, world");

    // Avoids the cost of zero-filling the buffer first that resize() would do.
    return 0;
}
