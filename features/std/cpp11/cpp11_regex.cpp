// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: <regex>: build a regex_basic / regex_match / regex_search / regex_replace pipeline. ECMAScript syntax by default.
// reference: https://en.cppreference.com/w/cpp/regex

#include <cassert>
#include <regex>
#include <string>

int main() {
    std::regex pattern(R"((\d{4})-(\d{2})-(\d{2}))");

    std::string s = "today is 2026-05-05 actually";

    // regex_search finds the first match anywhere in s.
    std::smatch m;
    bool found = std::regex_search(s, m, pattern);
    assert(found);
    assert(m[0].str() == "2026-05-05");
    assert(m[1].str() == "2026");
    assert(m[2].str() == "05");
    assert(m[3].str() == "05");

    // regex_match requires the WHOLE string to match.
    assert(!std::regex_match(s, pattern));
    assert( std::regex_match("2026-05-05", pattern));

    // regex_replace produces a transformed copy.
    auto out = std::regex_replace(s, pattern, "[$1/$2/$3]");
    assert(out == "today is [2026/05/05] actually");
    return 0;
}
