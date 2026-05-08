// description: C++17 added shared_ptr<T[]> -- shared ownership of a dynamically-allocated array, with operator[] and the correct delete[] on destruction. Pre-C++17, only unique_ptr<T[]> had this.
// reference: https://en.cppreference.com/w/cpp/memory/shared_ptr

#include "support/demo.hpp"
#include <memory>
#include <numeric>

int main() {
    demo::title("C++17 shared_ptr<T[]>");

    // C++20 made std::make_shared<T[]>(n) work; for C++17 the canonical
    // shape is constructing the array via 'new[]' and giving shared_ptr
    // the [] specialisation so it picks delete[] at destruction.
    std::shared_ptr<int[]> buf(new int[5]());      // value-initialised to 0
    DEMO_ASSERT(buf[0] == 0 && buf[4] == 0);

    // operator[] is a real array subscript -- not a struct-style get<I>().
    for (int i = 0; i < 5; ++i) buf[i] = i * i;
    demo::value("sum of squares 0..4",
                std::accumulate(&buf[0], &buf[0] + 5, 0));

    // Reference counting works as usual; copies all see the same array.
    auto buf2 = buf;
    DEMO_ASSERT(buf.use_count() == 2);
    buf2[0] = 999;
    DEMO_ASSERT(buf[0] == 999);

    // The array deleter is automatic because of the [] specialisation:
    // when the last shared_ptr goes out of scope, delete[] is called.
    // Pre-C++17, the equivalent required passing an explicit deleter:
    //   std::shared_ptr<int> bad(new int[5], std::default_delete<int[]>{});
    return 0;
}
