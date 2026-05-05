// gcc-test: std=c++20 min-gcc=14 topic=chrono experimental=false
// description: <chrono> grew calendar types: year_month_day, weekday, sys_days, ymd_weekday arithmetic.
// reference: https://en.cppreference.com/w/cpp/chrono

#include <cassert>
#include <chrono>

int main() {
    using namespace std::chrono;

    constexpr year_month_day ymd{year{2026}, May, day{5}};
    static_assert(ymd.year() == year{2026});
    static_assert(ymd.month() == May);
    static_assert(ymd.day() == day{5});

    // Weekday of an explicit date.
    constexpr weekday wd{sys_days{ymd}};
    // 2026-05-05 is a Tuesday.
    static_assert(wd == Tuesday);

    // Calendar arithmetic: same date, one month later.
    constexpr auto ymd_plus = ymd + months{1};
    static_assert(ymd_plus.month() == June);
    static_assert(ymd_plus.day() == day{5});

    // sys_time arithmetic: 24 hours earlier.
    constexpr sys_days today = sys_days{ymd};
    constexpr sys_days yesterday = today - days{1};
    constexpr year_month_day y_ymd{yesterday};
    static_assert(y_ymd.day() == day{4});
    return 0;
}
