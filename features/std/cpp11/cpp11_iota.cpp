// description: std::iota fills a range with sequential, incrementing values starting at a given seed -- the standard "0..N-1" generator.
// reference: https://en.cppreference.com/w/cpp/algorithm/iota

#include "support/demo.hpp"
#include <array>
#include <numeric>
#include <vector>

int main() {
    demo::title("C++11 iota");

    std::vector<int> v(5);
    std::iota(v.begin(), v.end(), 0);
    demo::range("0..4", v);

    // Works with any incrementable type, including pointers/iterators (rare)
    // and char (more common: ASCII tables).
    std::array<char, 5> letters{};
    std::iota(letters.begin(), letters.end(), 'a');
    demo::range("a..e", letters);

    // Common use: build a "permutation index" vector to sort indirectly.
    std::vector<int> data{50, 10, 40, 20, 30};
    std::vector<std::size_t> idx(data.size());
    std::iota(idx.begin(), idx.end(), 0);
    DEMO_ASSERT(idx[0] == 0 && idx[4] == 4);

    return 0;
}
