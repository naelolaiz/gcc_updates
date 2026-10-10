// description: #embed pastes a file's bytes into the program at compile time as a comma-separated list of integers, so binary resources need no external generator.
// reference: https://en.cppreference.com/w/cpp/preprocessor/embed
// why: Shaders, certificates, test fixtures, or lookup tables can ship inside the binary straight from their source files.
// before: xxd -i or a custom build step generated a C array, or the program loaded the file at run time.
// pitfall: The bytes are fixed when the translation unit is compiled; a changed resource needs a rebuild, and huge files cost compile time.

#include "support/demo.hpp"
#include <string_view>

// Embed this very source file: __FILE__ expands to its path, and limit(14)
// keeps only the first 14 bytes.
constexpr char self[] = {
#embed __FILE__ limit(14)
};

int main() {
    demo::title("C++26 #embed");
    constexpr std::string_view head{self, sizeof self};
    static_assert(head == "// description");
    demo::text("first bytes of this file", std::string(head));
    return 0;
}
