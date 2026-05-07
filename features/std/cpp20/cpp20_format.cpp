// gcc-test: std=c++20 min-gcc=14 topic=format experimental=false
// description: std::format provides Python-style typesafe formatting; std::format_to writes into an output iterator.
// reference: https://en.cppreference.com/w/cpp/utility/format/format

#include <cassert>
#include <format>
#include <string>

int main() {
    auto s1 = std::format("{} = {:.3f}", "pi", 3.14159265);
    assert(s1 == "pi = 3.142");

    auto s2 = std::format("[{:>6}|{:<6}|{:^6}]", "a", "b", "c");
    assert(s2 == "[     a|b     |  c   ]");

    auto s3 = std::format("hex={:#x} bin={:#b} oct={:#o}", 255, 5, 8);
    assert(s3 == "hex=0xff bin=0b101 oct=010");

    // format_to writes incrementally.
    std::string out;
    std::format_to(std::back_inserter(out), "{}-{}-{}", 2026, 5, 5);
    assert(out == "2026-5-5");

    // Positional args.
    auto s4 = std::format("{0} {1} {0}", "ping", "pong");
    assert(s4 == "ping pong ping");

    return 0;
}
