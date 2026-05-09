// description: <spanstream> wraps a std::span as the buffer for an iostream -- ispanstream / ospanstream / spanstream do the same job as stringstream but with no allocation, no copy, and no implicit lifetime ownership.
// reference: https://en.cppreference.com/w/cpp/header/spanstream

#include "support/demo.hpp"
#include <span>
#include <spanstream>
#include <string>

int main() {
    demo::title("C++23 spanstream");

    // Output: write into a fixed buffer; .span() reports the populated prefix.
    char obuf[64]{};
    std::ospanstream out(obuf);
    out << "answer=" << 42 << ',' << std::hex << 255;
    auto written = out.span();          // a span<char> over the bytes used
    std::string view(written.data(), written.size());
    demo::text("ospanstream wrote", view);
    DEMO_ASSERT(view.find("answer=42") != std::string::npos);
    DEMO_ASSERT(view.find(",ff") != std::string::npos);

    // Input: parse from a fixed buffer with no allocation.
    char input_buf[] = "10 3.14 hello";
    std::ispanstream in(std::span<char>{input_buf, sizeof(input_buf) - 1});
    int    n;
    double d;
    std::string word;
    in >> n >> d >> word;
    DEMO_ASSERT(n == 10);
    DEMO_ASSERT(d == 3.14);
    DEMO_ASSERT(word == "hello");
    demo::text("parsed", "n=10, d=3.14, word=hello");
    return 0;
}
