// description: C++17 polished <type_traits>: _v helpers (no ::value), _t aliases (no typename ::type), std::void_t for SFINAE detection, std::conjunction/disjunction/negation for short-circuiting trait composition, and std::is_invocable / std::invoke_result.
// reference: https://en.cppreference.com/w/cpp/header/type_traits

#include "support/demo.hpp"
#include <string>
#include <type_traits>

// _v helpers: std::is_integral_v<T> instead of std::is_integral<T>::value.
static_assert(std::is_integral_v<int>);
static_assert(std::is_same_v<std::remove_const_t<const int>, int>);

// std::void_t<...> ignores its arguments and yields void; it's the standard
// idiom for "does expression X compile?" detection.
template <typename, typename = void>
struct has_size_member : std::false_type {};

template <typename T>
struct has_size_member<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

static_assert(has_size_member<std::string>::value);
static_assert(!has_size_member<int>::value);

// std::conjunction = AND with short-circuit: stops instantiating once it sees
// a false. std::disjunction is the OR equivalent. std::negation flips one.
template <typename T>
constexpr bool small_arithmetic_v =
    std::conjunction_v<std::is_arithmetic<T>,
                       std::bool_constant<(sizeof(T) <= 4)>>;

static_assert(small_arithmetic_v<int>);
static_assert(small_arithmetic_v<float>);
static_assert(!small_arithmetic_v<double>);
static_assert(!small_arithmetic_v<std::string>);

template <typename T>
constexpr bool int_or_string_v =
    std::disjunction_v<std::is_same<T, int>, std::is_same<T, std::string>>;
static_assert(int_or_string_v<int>);
static_assert(int_or_string_v<std::string>);
static_assert(!int_or_string_v<double>);

static_assert(std::negation_v<std::is_pointer<int>>);

// std::is_invocable / invoke_result_t replace the old result_of and answer
// "can I call F with these args, and what's the return type?".
auto add = [](int a, int b) { return a + b; };
static_assert(std::is_invocable_v<decltype(add), int, int>);
static_assert(!std::is_invocable_v<decltype(add), int, std::string>);
static_assert(std::is_same_v<std::invoke_result_t<decltype(add), int, int>, int>);

int main() {
    demo::title("C++17 type trait helpers");
    demo::value("has_size_member<string>", has_size_member<std::string>::value);
    demo::value("has_size_member<int>",    has_size_member<int>::value);
    demo::value("small_arithmetic<int>",   small_arithmetic_v<int>);
    demo::value("small_arithmetic<double>",small_arithmetic_v<double>);
    demo::value("invocable add(int,int)",  std::is_invocable_v<decltype(add), int, int>);
    return 0;
}
