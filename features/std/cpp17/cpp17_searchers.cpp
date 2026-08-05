// description: <functional> gained Boyer-Moore(-Horspool) searchers: preprocess the needle once, then hand the searcher to std::search for every haystack -- repeated substring searches get much cheaper.
// reference: https://en.cppreference.com/w/cpp/utility/functional/boyer_moore_searcher

#include "support/demo.hpp"
#include <algorithm>
#include <functional>
#include <string>

int main() {
    demo::title("C++17 Boyer-Moore searchers");

    const std::string haystack =
        "the quick brown fox jumps over the lazy dog; "
        "the quick brown fox naps afterwards";
    const std::string needle = "brown fox";

    // Preprocess the needle once...
    std::boyer_moore_searcher searcher(needle.begin(), needle.end());

    // ...then search as many times as needed.
    auto first = std::search(haystack.begin(), haystack.end(), searcher);
    DEMO_ASSERT(first != haystack.end());
    demo::value("first match at", first - haystack.begin());

    auto second = std::search(first + 1, haystack.end(), searcher);
    DEMO_ASSERT(second != haystack.end() && second > first);

    // Horspool variant: smaller tables, similar speed in practice.
    std::boyer_moore_horspool_searcher horspool(needle.begin(), needle.end());
    DEMO_ASSERT(std::search(haystack.begin(), haystack.end(), horspool) == first);

    // default_searcher wraps plain search behaviour behind the same API.
    std::default_searcher plain(needle.begin(), needle.end());
    DEMO_ASSERT(std::search(haystack.begin(), haystack.end(), plain) == first);

    const std::string missing = "purple fox";
    std::boyer_moore_searcher none(missing.begin(), missing.end());
    DEMO_ASSERT(std::search(haystack.begin(), haystack.end(), none) ==
                haystack.end());
    return 0;
}
