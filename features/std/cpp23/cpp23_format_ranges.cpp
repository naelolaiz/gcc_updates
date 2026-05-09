// description: C++23 made every range formattable by std::format -- pass any container/view through "{}", customise per-element with the nested format spec "{::}", or pick join_with for a custom separator.
// reference: https://en.cppreference.com/w/cpp/utility/format/range_formatter

#include "support/demo.hpp"
#include <format>
#include <map>
#include <string>
#include <vector>

int main() {
    demo::title("C++23 format ranges");

    std::vector<int> nums{1, 2, 3, 4};

    // Default range output: brackets + comma separator.
    auto s1 = std::format("{}", nums);
    demo::text("default", s1);                     // [1, 2, 3, 4]

    // The nested format spec after a colon applies to *each element*. Hex
    // each int via "{::#x}".
    auto s2 = std::format("{::#x}", nums);
    demo::text("hex elements", s2);                // [0x1, 0x2, 0x3, 0x4]

    // Field width per element + zero-padded.
    auto s3 = std::format("{::04}", nums);
    demo::text("zero-padded", s3);                 // [0001, 0002, 0003, 0004]

    // 'n' on the *outer* spec drops the brackets.
    auto s4 = std::format("{:n}", nums);
    demo::text("no brackets", s4);                 // 1, 2, 3, 4

    // Maps (and other associative containers) format as a list of pairs.
    std::map<std::string, int> m{{"alpha", 1}, {"beta", 2}};
    auto s5 = std::format("{}", m);
    demo::text("map", s5);                         // {"alpha": 1, "beta": 2}

    // Range of strings: each element formatted as a string by default.
    std::vector<std::string> words{"foo", "bar"};
    auto s6 = std::format("{}", words);
    demo::text("strings", s6);                     // ["foo", "bar"]

    DEMO_ASSERT(s1.find("1, 2, 3, 4") != std::string::npos);
    return 0;
}
