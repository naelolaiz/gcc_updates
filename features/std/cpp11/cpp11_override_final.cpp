// description: 'override' makes the compiler verify that a method really overrides a virtual; 'final' forbids further overriding or inheritance.
// reference: https://en.cppreference.com/w/cpp/language/override

#include "support/demo.hpp"
#include <cassert>

struct Base {
    virtual int answer() const { return 0; }
    virtual ~Base() = default;
};

struct Mid : Base {
    int answer() const override { return 42; }   // verified
};

struct Leaf final : Mid {
    int answer() const override final { return 99; }   // no further overriding
};

// struct CannotInherit : Leaf {};   // would not compile -- Leaf is final

int main() {
    demo::title("C++11 override final");
    Mid m;
    Leaf l;
    Base* p = &m;
    DEMO_ASSERT(p->answer() == 42);
    p = &l;
    DEMO_ASSERT(p->answer() == 99);
    return 0;
}
