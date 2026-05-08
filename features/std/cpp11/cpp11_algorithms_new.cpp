// description: C++11 added a batch of <algorithm> conveniences: all_of/any_of/none_of, find_if_not, copy_if/copy_n, std::move (algorithm form), shuffle, is_sorted/is_partitioned, partition_point, minmax/minmax_element.
// reference: https://en.cppreference.com/w/cpp/algorithm

#include "support/demo.hpp"
#include <algorithm>
#include <random>
#include <vector>

int main() {
    demo::title("C++11 new algorithms");

    std::vector<int> v{1, 2, 3, 4, 5, 6};

    // Predicate scans -- replace the old "for + flag" idiom.
    DEMO_ASSERT(std::all_of(v.begin(), v.end(), [](int x){ return x > 0; }));
    DEMO_ASSERT(std::any_of(v.begin(), v.end(), [](int x){ return x % 2 == 0; }));
    DEMO_ASSERT(std::none_of(v.begin(), v.end(), [](int x){ return x > 100; }));

    auto first_odd = std::find_if_not(v.begin(), v.end(),
                                      [](int x){ return x % 2 == 0; });
    DEMO_ASSERT(*first_odd == 1);

    // copy_if / copy_n -- selective and bounded copies.
    std::vector<int> evens;
    std::copy_if(v.begin(), v.end(), std::back_inserter(evens),
                 [](int x){ return x % 2 == 0; });
    demo::range("evens", evens);

    std::vector<int> first_three(3);
    std::copy_n(v.begin(), 3, first_three.begin());
    demo::range("first 3", first_three);

    // std::move algorithm: like std::copy but uses move-assignment per element.
    std::vector<std::vector<int>> src{{1,2}, {3,4}};
    std::vector<std::vector<int>> dst(src.size());
    std::move(src.begin(), src.end(), dst.begin());
    DEMO_ASSERT(dst[0].size() == 2 && src[0].empty());

    // Shuffled order-checking.
    std::vector<int> sorted_v{1,2,3,4,5};
    DEMO_ASSERT(std::is_sorted(sorted_v.begin(), sorted_v.end()));

    std::vector<int> partitioned{2,4,6,1,3,5};   // evens then odds
    auto pred = [](int x){ return x % 2 == 0; };
    DEMO_ASSERT(std::is_partitioned(partitioned.begin(), partitioned.end(), pred));
    auto split = std::partition_point(partitioned.begin(), partitioned.end(), pred);
    DEMO_ASSERT(*split == 1);

    // shuffle replaces the deprecated random_shuffle (which used rand()).
    std::mt19937 rng(0xC0FFEE);
    std::shuffle(sorted_v.begin(), sorted_v.end(), rng);
    demo::range("shuffled", sorted_v);

    // minmax_element returns iterators to both extremes in a single pass.
    auto mm = std::minmax_element(v.begin(), v.end());
    DEMO_ASSERT(*mm.first == 1 && *mm.second == 6);
    return 0;
}
