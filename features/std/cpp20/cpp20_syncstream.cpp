// description: std::osyncstream interleaves output safely from multiple threads -- each scope's text is emitted as one block.
// reference: https://en.cppreference.com/w/cpp/io/basic_osyncstream

#include "support/demo.hpp"
#include <cassert>
#include <sstream>
#include <syncstream>
#include <thread>
#include <vector>

int main() {
    demo::title("C++20 syncstream");
    std::ostringstream sink;

    {
        std::vector<std::jthread> ts;
        for (int i = 0; i < 8; ++i) {
            ts.emplace_back([i, &sink] {
                // The osyncstream buffers writes and emits them atomically
                // when the object is destroyed (here, at end of scope).
                std::osyncstream(sink) << "thread-" << i << ":start;"
                                       << "thread-" << i << ":end;\n";
            });
        }
    }

    // Each thread contributed one full line containing both ":start;" and ":end;"
    // adjacent. Without osyncstream the bytes from different threads could
    // interleave inside a line.
    auto s = sink.str();
    for (int i = 0; i < 8; ++i) {
        std::string needle = "thread-" + std::to_string(i) +
                             ":start;thread-" + std::to_string(i) + ":end;";
        DEMO_ASSERT(s.find(needle) != std::string::npos);
    }
    return 0;
}
