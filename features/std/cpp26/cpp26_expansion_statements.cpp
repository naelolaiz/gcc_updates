// description: Expansion statements use template for to instantiate a statement once per compile-time element, preserving each element's distinct type or value.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p1306r5.html
// why: Heterogeneous compile-time sequences can be processed without index_sequence or recursive templates.
// before: Generate indices, expand a helper function, and recover every element with std::get.
// pitfall: This is compile-time expansion, not a runtime loop.

#include "support/demo.hpp"

consteval int first_element_sum(auto const&... containers) {
    int result = 0;
    template for (auto const& container : {containers...}) {
        result += container[0];
    }
    return result;
}

int main() {
    demo::title("C++26 expansion statements");
    constexpr int first[]{1, 2, 3};
    constexpr int second[]{4, 5};
    constexpr int third[]{7};
    static_assert(first_element_sum(first, second, third) == 12);
    demo::text("check", "template for expanded three statements");
    return 0;
}
