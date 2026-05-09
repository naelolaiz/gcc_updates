// description: std::integer_sequence<T, Is...> is a compile-time pack of integer constants; std::make_index_sequence<N> generates 0..N-1, the canonical way to expand a tuple/array into argument lists.
// reference: https://en.cppreference.com/w/cpp/utility/integer_sequence

#include "support/demo.hpp"
#include <array>
#include <cstddef>
#include <tuple>
#include <utility>

// Classic use: turn make_index_sequence<N>{} into a parameter pack of indices,
// then expand it -- here, to call f with each tuple element as a separate arg
// (this is exactly how std::apply is implemented in the standard library).
template <typename F, typename Tuple, std::size_t... Is>
constexpr auto apply_impl(F&& f, Tuple&& t, std::index_sequence<Is...>) {
    return f(std::get<Is>(std::forward<Tuple>(t))...);
}

template <typename F, typename Tuple>
constexpr auto my_apply(F&& f, Tuple&& t) {
    constexpr auto N = std::tuple_size<typename std::decay<Tuple>::type>::value;
    return apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
                      std::make_index_sequence<N>{});
}

// Build a constexpr lookup table at compile time: each Is becomes a slot.
template <std::size_t... Is>
constexpr std::array<int, sizeof...(Is)> squares_impl(std::index_sequence<Is...>) {
    return {{ static_cast<int>(Is * Is)... }};
}
template <std::size_t N>
constexpr std::array<int, N> squares() {
    return squares_impl(std::make_index_sequence<N>{});
}

// std::index_sequence_for<Args...> is shorthand for make_index_sequence<sizeof...(Args)>.
template <typename... Args, std::size_t... Is>
void print_pack_impl(const std::tuple<Args...>& t, std::index_sequence<Is...>) {
    int dummy[] = { (demo::value("[" + std::to_string(Is) + "]", std::get<Is>(t)), 0)... };
    (void)dummy;
}
template <typename... Args>
void print_pack(const std::tuple<Args...>& t) {
    print_pack_impl(t, std::index_sequence_for<Args...>{});
}

int main() {
    demo::title("C++14 integer sequence");

    auto sum3 = [](int a, int b, int c) { return a + b + c; };
    DEMO_ASSERT(my_apply(sum3, std::make_tuple(1, 2, 3)) == 6);

    constexpr auto sq = squares<5>();
    static_assert(sq[0] == 0 && sq[3] == 9 && sq[4] == 16, "");
    demo::range("squares<5>", sq);

    print_pack(std::make_tuple(10, 20, 30));
    return 0;
}
