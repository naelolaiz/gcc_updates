// description: A structured binding can introduce a pack, turning a tuple-like object into named pack elements without std::apply or index_sequence.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p1061r10.html
// why: Generic tuple algorithms can use normal pack expansions directly in the function body.
// before: std::apply with a nested generic lambda or a separate index_sequence helper.
// pitfall: The adopted form introduces packs only inside templated entities.

#include "support/demo.hpp"
#include <tuple>

template <class Tuple>
constexpr auto sum_tuple(Tuple tuple) {
    auto [...elements] = tuple;
    return (... + elements);
}

int main() {
    demo::title("C++26 structured binding pack");
    static_assert(sum_tuple(std::tuple{1, 2, 3, 4}) == 10);
    DEMO_ASSERT(sum_tuple(std::tuple{1.5, 2.5}) == 4.0);
    return 0;
}
