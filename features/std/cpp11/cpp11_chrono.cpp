// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: <chrono> models durations and time_points with type-safe units; arithmetic between mismatched units is rejected.
// reference: https://en.cppreference.com/w/cpp/chrono

#include "support/demo.hpp"
#include <cassert>
#include <chrono>
#include <thread>

int main() {
    demo::title("C++11 chrono");
    using namespace std::chrono;

    auto sec  = seconds(2);
    auto ms   = milliseconds(500);
    auto sum  = sec + ms;                  // 2500ms

    DEMO_ASSERT(duration_cast<milliseconds>(sum).count() == 2500);
    DEMO_ASSERT(duration_cast<seconds>(sum).count() == 2);

    // Cross-clock arithmetic with the steady_clock (monotonic).
    auto t0 = steady_clock::now();
    std::this_thread::sleep_for(milliseconds(2));
    auto t1 = steady_clock::now();

    auto elapsed = duration_cast<microseconds>(t1 - t0);
    DEMO_ASSERT(elapsed.count() >= 1000);   // at least ~1ms

    // C++14 added literal suffixes (1s, 1ms); in pure C++11 you spell out
    // milliseconds(...).
    return 0;
}
