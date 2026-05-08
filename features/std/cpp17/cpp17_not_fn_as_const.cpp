// description: std::not_fn(p) returns a callable that negates p (replaces the deprecated not1/not2 binders); std::as_const(x) returns a const reference, forcing the const overload of begin()/find()/etc. without copying.
// reference: https://en.cppreference.com/w/cpp/utility/functional/not_fn

#include "support/demo.hpp"
#include <algorithm>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

bool is_even(int x) { return x % 2 == 0; }

int main() {
    demo::title("C++17 not_fn / as_const");

    std::vector<int> v{1, 2, 3, 4, 5, 6};

    // Without not_fn you'd write a wrapper lambda. With it, any predicate
    // (function, member-fn pointer, lambda, std::function, ...) negates
    // uniformly.
    auto first_odd = std::find_if(v.begin(), v.end(), std::not_fn(is_even));
    DEMO_ASSERT(*first_odd == 1);

    auto count_odd = std::count_if(v.begin(), v.end(), std::not_fn(is_even));
    DEMO_ASSERT(count_odd == 3);

    // std::as_const turns 'T&' into 'const T&'. The classic use: get the
    // const overload of begin()/end() so you iterate cbegin/cend without
    // sprinkling cbegin everywhere.
    std::vector<int> mut_vec{10, 20};
    auto& cref = std::as_const(mut_vec);
    static_assert(std::is_same_v<decltype(cref), const std::vector<int>&>);
    static_assert(std::is_same_v<decltype(cref.begin()),
                                 std::vector<int>::const_iterator>);

    // as_const protects against accidental modification through a binding.
    // mut_vec.push_back(30);             // OK (mutable)
    // std::as_const(mut_vec).push_back(30);  // would not compile (const ref)

    // Note: std::as_const intentionally has no rvalue overload (= deleted)
    // so std::as_const(make_vec()) is a compile error -- you'd be holding
    // a const reference to a temporary.

    demo::text("not_fn", "negate any callable without writing a lambda");
    demo::text("as_const", "force const overload selection on a mutable object");
    return 0;
}
