// description: = delete("reason") attaches an explanation to a deleted function; a caller's error message then says WHY it is deleted and what to use instead of a bare 'use of deleted function'.
// reference: https://en.cppreference.com/w/cpp/language/function

#include "support/demo.hpp"
#include <string_view>

// Calling the deleted overload now produces:
//   error: use of deleted function 'void parse_config(const char*)':
//          use parse_config(std::string_view) so the length is tracked
void parse_config(const char*) = delete("use parse_config(std::string_view) so the length is tracked");
bool parse_config(std::string_view text) { return !text.empty(); }

struct PinnedBuffer {
    PinnedBuffer() = default;
    PinnedBuffer(const PinnedBuffer&) = delete("PinnedBuffer is address-stable; pass it by reference");
    PinnedBuffer& operator=(const PinnedBuffer&) = delete("PinnedBuffer is address-stable; pass it by reference");
};

int main() {
    demo::title("C++26 delete with reason");

    DEMO_ASSERT(parse_config(std::string_view("key=value")));
    // parse_config("key=value");  // error with the reason text above

    PinnedBuffer buf;
    (void)buf;
    // PinnedBuffer copy = buf;    // error: ...address-stable; pass it by reference
    demo::text("status", "deleted overloads carry their reasons");
    return 0;
}
