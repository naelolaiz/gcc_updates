// description: GCC 15's libstdc++ formats ranges and tuples (P2286R8, P2585R1), so std::println("{}", v) prints [1, 2, 3] directly -- GCC 14 rejected it because std::vector had no std::formatter.
// reference: https://gcc.gnu.org/gcc-15/changes.html

#include "support/demo.hpp"
#include <format>
#include <print>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

int main() {
    demo::title("GCC 15: print ranges and tuples");

    std::vector<int> v{1, 2, 3};
    std::println("  println a vector: {}", v);

    DEMO_ASSERT(std::format("{}", v) == "[1, 2, 3]");
    DEMO_ASSERT(std::format("{}", std::pair{1, 'x'}) == "(1, 'x')");
    DEMO_ASSERT(std::format("{}", std::tuple{2, std::string{"two"}}) == "(2, \"two\")");
    return 0;
}
