// gcc-test: std=c++23 min-gcc=14 topic=templates experimental=false
// description: operator[] now accepts multiple arguments -- the natural syntax for tensor / matrix types.
// reference: https://en.cppreference.com/w/cpp/language/operators#Subscript_operator

#include "support/demo.hpp"
#include <array>
#include <cassert>

template <typename T, std::size_t Rows, std::size_t Cols>
struct Matrix {
    std::array<T, Rows * Cols> data{};

    constexpr T& operator[](std::size_t r, std::size_t c) {
        return data[r * Cols + c];
    }
    constexpr const T& operator[](std::size_t r, std::size_t c) const {
        return data[r * Cols + c];
    }
};

int main() {
    demo::title("C++23 multidim subscript");
    Matrix<int, 2, 3> m{};
    m[0, 0] = 1;  m[0, 1] = 2;  m[0, 2] = 3;
    m[1, 0] = 4;  m[1, 1] = 5;  m[1, 2] = 6;

    // The assert macro is a comma-expression, so multi-arg subscript needs
    // an extra paren to keep it as a single argument to DEMO_ASSERT().
    DEMO_ASSERT((m[0, 0] == 1));
    DEMO_ASSERT((m[1, 2] == 6));

    int sum = 0;
    for (std::size_t r = 0; r < 2; ++r)
        for (std::size_t c = 0; c < 3; ++c)
            sum += m[r, c];
    DEMO_ASSERT(sum == 21);
    return 0;
}
