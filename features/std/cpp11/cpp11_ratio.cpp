// description: std::ratio<N, D> is a compile-time rational number; <ratio> exposes ratio_add/subtract/multiply/divide and ratio_equal/less for arithmetic on types.
// reference: https://en.cppreference.com/w/cpp/numeric/ratio/ratio

#include "support/demo.hpp"
#include <chrono>
#include <ratio>
#include <type_traits>

// std::ratio is a *type* whose value is fixed at compile time. The library
// reduces it: ratio<2, 4> and ratio<1, 2> are the same type after reduction.
using half      = std::ratio<1, 2>;
using third     = std::ratio<1, 3>;
using two_quart = std::ratio<2, 4>;

static_assert(half::num == 1 && half::den == 2, "");
static_assert(std::ratio_equal<half, two_quart>::value, "");

// Arithmetic operators on ratios produce new ratio types.
using sum_h_t  = std::ratio_add<half, third>::type;        // 5/6
using diff_h_t = std::ratio_subtract<half, third>::type;   // 1/6
using prod     = std::ratio_multiply<half, third>::type;   // 1/6
using quot     = std::ratio_divide<half, third>::type;     // 3/2

static_assert(sum_h_t::num  == 5 && sum_h_t::den  == 6, "");
static_assert(diff_h_t::num == 1 && diff_h_t::den == 6, "");
static_assert(prod::num     == 1 && prod::den     == 6, "");
static_assert(quot::num     == 3 && quot::den     == 2, "");

// Comparisons too.
static_assert(std::ratio_less<third, half>::value, "");
static_assert(!std::ratio_less<half, third>::value, "");

// The motivating use: <chrono> defines duration<Rep, Period> where Period is
// a std::ratio. std::milli == ratio<1, 1000>, std::kilo == ratio<1000, 1>.
// All unit conversions are pure compile-time arithmetic on ratio types.
static_assert(std::is_same<std::milli, std::ratio<1, 1000>>::value, "");
static_assert(std::is_same<std::kilo,  std::ratio<1000, 1>>::value, "");

using minutes_in_seconds = std::chrono::duration<long, std::ratio<60>>;
static_assert(std::is_same<minutes_in_seconds, std::chrono::minutes>::value, "");

int main() {
    demo::title("C++11 ratio");
    demo::value("half + third num",  static_cast<int>(sum_h_t::num));
    demo::value("half + third den",  static_cast<int>(sum_h_t::den));
    demo::value("half * third num",  static_cast<int>(prod::num));
    demo::value("half * third den",  static_cast<int>(prod::den));
    demo::value("milli is 1/1000",   std::ratio_equal<std::milli, std::ratio<1, 1000>>::value);

    // ratio drives chrono unit conversions: 2 minutes -> 120 seconds.
    auto two_min  = std::chrono::minutes(2);
    auto in_secs  = std::chrono::duration_cast<std::chrono::seconds>(two_min);
    DEMO_ASSERT(in_secs.count() == 120);
    return 0;
}
