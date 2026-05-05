// gcc-test: std=c++11 min-gcc=13 topic=stl experimental=false
// description: unique_ptr (sole ownership) / shared_ptr (refcounted) / weak_ptr (non-owning observer); std::make_shared is the preferred construction.
// reference: https://en.cppreference.com/w/cpp/memory/unique_ptr

#include <cassert>
#include <memory>

struct Resource {
    int value;
    static int alive;
    explicit Resource(int v) : value(v) { ++alive; }
    ~Resource() { --alive; }
};
int Resource::alive = 0;

int main() {
    {
        std::unique_ptr<Resource> u(new Resource(7));
        assert(u->value == 7);
        assert(Resource::alive == 1);

        // Move ownership; u becomes null.
        std::unique_ptr<Resource> v = std::move(u);
        assert(!u);
        assert(v->value == 7);
    }
    assert(Resource::alive == 0);

    {
        auto a = std::make_shared<Resource>(42);     // single allocation for ctrl-block + obj
        auto b = a;                                   // refcount += 1
        assert(a.use_count() == 2);

        std::weak_ptr<Resource> w(a);                 // non-owning observer
        assert(!w.expired());
        if (auto locked = w.lock()) {
            assert(locked->value == 42);
        }

        a.reset();                                    // refcount -> 1
        b.reset();                                    // refcount -> 0, destroys
        assert(w.expired());
    }
    assert(Resource::alive == 0);
    return 0;
}
