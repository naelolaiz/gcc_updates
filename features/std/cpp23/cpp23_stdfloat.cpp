// description: <stdfloat> defines fixed-width floating types: std::float32_t/float64_t are guaranteed IEEE binary32/64 (unlike float/double, which only usually are), plus float16_t/bfloat16_t where the target has them.
// reference: https://en.cppreference.com/w/cpp/types/floating-point

#include "support/demo.hpp"
#include <stdfloat>
#include <type_traits>

int main() {
    demo::title("C++23 fixed-width floating point");

    std::float64_t a = 1.5f64;  // literal suffixes match the type names
    std::float32_t b = 0.25f32;
    static_assert(sizeof(a) == 8 && sizeof(b) == 4);

    // Distinct types from double/float (usually same representation on
    // x86-64) -- overloads can distinguish them.
    static_assert(!std::is_same_v<std::float64_t, double>);

    double sum = static_cast<double>(a) + static_cast<double>(b);
    DEMO_ASSERT(sum == 1.75);

    // Optional types advertise themselves via feature-test macros.
#if defined(__STDCPP_FLOAT16_T__)
    std::float16_t h = 0.5f16;  // storage-efficient half precision
    DEMO_ASSERT(static_cast<float>(h) == 0.5f);
    demo::value("float16 supported", true);
#endif
#if defined(__STDCPP_BFLOAT16_T__)
    std::bfloat16_t bf = 1.5bf16;  // ML-style truncated binary32
    DEMO_ASSERT(static_cast<float>(bf) == 1.5f);
    demo::value("bfloat16 supported", true);
#endif
    return 0;
}
