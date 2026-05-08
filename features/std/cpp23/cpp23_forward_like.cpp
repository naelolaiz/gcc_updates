// description: std::forward_like<Self>(x) returns x with the cv-qualification AND value category of Self -- the cleanest way to forward an inner member from a deducing-this method without writing four overloads.
// reference: https://en.cppreference.com/w/cpp/utility/forward_like

#include "support/demo.hpp"
#include <string>
#include <type_traits>
#include <utility>

struct Box {
    std::string data{"hello"};

    // 'Self' captures both ref-ness AND const-ness of the *this expression
    // at the call site. forward_like applies that envelope to data, so:
    //   const Box  b; b.payload();  -> const string&
    //   Box        b; b.payload();  -> string&
    //   std::move(b).payload();     -> string&&
    template <typename Self>
    auto&& payload(this Self&& self) {
        return std::forward_like<Self>(self.data);
    }
};

int main() {
    demo::title("C++23 forward_like");

    Box b;
    static_assert(std::is_same_v<decltype(b.payload()), std::string&>);
    static_assert(std::is_same_v<decltype(std::as_const(b).payload()),
                                 const std::string&>);
    static_assert(std::is_same_v<decltype(std::move(b).payload()),
                                 std::string&&>);

    // Exercise the rvalue path: payload() of an rvalue Box yields a string&&,
    // so the move-construct of `taken` actually moves out of b.data.
    std::string taken = std::move(b).payload();
    DEMO_ASSERT(taken == "hello");
    demo::text("after move", b.data.empty() ? "empty (moved-from)" : b.data);

    // Pre-C++23, the equivalent took 4 overloads (T&, const T&, T&&, const T&&)
    // or a perfect-forwarding helper. forward_like turns it into one line.
    return 0;
}
