// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: 'override' makes the compiler verify that a method really overrides a virtual; 'final' forbids further overriding or inheritance.
// reference: https://en.cppreference.com/w/cpp/language/override

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
    Mid m;
    Leaf l;
    Base* p = &m;
    assert(p->answer() == 42);
    p = &l;
    assert(p->answer() == 99);
    return 0;
}
