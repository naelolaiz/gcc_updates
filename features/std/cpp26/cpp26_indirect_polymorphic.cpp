// description: std::indirect gives heap-backed values deep-copy semantics; std::polymorphic extends that model to copyable polymorphic class hierarchies.
// reference: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p3019r14.html
// why: Recursive and polymorphic objects can retain ordinary value semantics.
// before: unique_ptr plus a hand-written clone operation and copy constructor.
// pitfall: Copying performs an allocation and a deep copy rather than sharing identity.

#include "support/demo.hpp"
#include <memory>
#include <string>

struct Shape {
    virtual ~Shape() = default;
    virtual int area() const = 0;
};

struct Square final : Shape {
    explicit Square(int side) : side(side) {}
    int area() const override { return side * side; }
    int side;
};

int main() {
    demo::title("C++26 indirect and polymorphic");

    std::indirect<std::string> first(std::in_place, "value");
    auto second = first;
    second->append(" copy");
    DEMO_ASSERT(*first == "value");
    DEMO_ASSERT(*second == "value copy");

    std::polymorphic<Shape> shape(std::in_place_type<Square>, 6);
    auto shape_copy = shape;
    DEMO_ASSERT(shape->area() == 36);
    DEMO_ASSERT(shape_copy->area() == 36);
    return 0;
}
