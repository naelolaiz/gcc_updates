// description: Heterogeneous ordered lookup: with the transparent comparator std::less<>, set/map find() compares the query against keys directly -- no temporary key object per lookup.
// reference: https://en.cppreference.com/w/cpp/utility/functional/less_void

#include "support/demo.hpp"
#include <map>
#include <set>
#include <string>

namespace {

// A key type that can be compared against a plain int. With std::less<>
// (note: no template argument), find(7) uses these mixed comparisons and
// never constructs an Id. With the default std::less<Id> it wouldn't compile.
struct Id {
    int n;
};
bool operator<(Id a, Id b) { return a.n < b.n; }
bool operator<(Id a, int b) { return a.n < b; }
bool operator<(int a, Id b) { return a < b.n; }

}  // namespace

int main() {
    demo::title("C++14 heterogeneous lookup");

    // The everyday win: look up by string literal without materialising
    // a std::string for each query.
    std::set<std::string, std::less<>> words{"alpha", "beta", "gamma"};
    DEMO_ASSERT(words.find("beta") != words.end());
    DEMO_ASSERT(words.count("delta") == 0);

    std::map<std::string, int, std::less<>> price{{"tea", 2}, {"coffee", 3}};
    auto it = price.find("tea");
    DEMO_ASSERT(it != price.end() && it->second == 2);

    // Custom key type, queried by the raw int it wraps.
    std::set<Id, std::less<>> ids{{1}, {2}, {3}};
    DEMO_ASSERT(ids.find(2) != ids.end());
    DEMO_ASSERT(ids.find(9) == ids.end());
    demo::value("ids.count(3)", ids.count(3));
    return 0;
}
