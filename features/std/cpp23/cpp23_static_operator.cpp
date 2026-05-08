// gcc-test: std=c++23 min-gcc=14 topic=templates experimental=false
// description: operator() and operator[] can be 'static' -- no implicit object parameter, slightly cheaper to call.
// reference: https://en.cppreference.com/w/cpp/language/operators#Function_call_operator

#include "support/demo.hpp"
#include <cassert>

struct Negate {
    static constexpr int operator()(int x) { return -x; }
};

struct Indexer {
    static constexpr int operator[](int x) { return x * 10; }
};

int main() {
    demo::title("C++23 static operator");
    Negate neg;
    static_assert(neg(5) == -5);
    static_assert(Negate{}(7) == -7);
    static_assert(Negate::operator()(3) == -3);   // call without an instance

    Indexer idx;
    static_assert(idx[4] == 40);
    static_assert(Indexer::operator[](6) == 60);
    demo::text("check", "static assertions passed");
    return 0;
}
