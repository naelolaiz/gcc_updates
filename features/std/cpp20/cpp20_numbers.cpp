// description: <numbers> ships precise math constants typed as templates; no more M_PI macros.
// reference: https://en.cppreference.com/w/cpp/header/numbers

#include "support/demo.hpp"
#include <cassert>
#include <cmath>
#include <numbers>

int main() {
    demo::title("C++20 numbers");
    static_assert(std::numbers::pi > 3.141 && std::numbers::pi < 3.142);
    static_assert(std::numbers::e  > 2.718 && std::numbers::e  < 2.719);

    // Templated form lets you pick the precision.
    constexpr double tau_d = 2.0 * std::numbers::pi_v<double>;
    constexpr float  tau_f = 2.0f * std::numbers::pi_v<float>;
    static_assert(tau_d > 6.283 && tau_d < 6.284);
    static_assert(tau_f > 6.283f && tau_f < 6.284f);

    DEMO_ASSERT(std::abs(std::numbers::sqrt2 * std::numbers::sqrt2 - 2.0) < 1e-12);
    return 0;
}
