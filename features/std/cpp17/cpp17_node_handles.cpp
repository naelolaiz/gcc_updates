// description: C++17 gave map/set node handles -- extract() removes a node WITHOUT destroying it, and you can move it to another container with insert(node) or merge() (no per-element copy/move). Also try_emplace / insert_or_assign tighten map insertion semantics.
// reference: https://en.cppreference.com/w/cpp/container/map/extract

#include "support/demo.hpp"
#include <map>
#include <set>
#include <string>

int main() {
    demo::title("C++17 node handles + map insertion");

    // try_emplace: emplaces ONLY if the key is absent. The args are forwarded
    // to the value constructor, so the value isn't even built when the key
    // already exists -- unlike emplace, which always constructs the pair.
    std::map<std::string, std::string> cfg;
    cfg.try_emplace("host", "127.0.0.1");
    cfg.try_emplace("host", "should-not-overwrite");   // no-op
    DEMO_ASSERT(cfg["host"] == "127.0.0.1");

    // insert_or_assign: classic upsert. Returns a (iterator, bool) pair where
    // the bool is true if inserted (false if assigned over).
    auto [it, inserted] = cfg.insert_or_assign("port", "8080");
    DEMO_ASSERT(inserted);
    auto [it2, inserted2] = cfg.insert_or_assign("port", "9090");
    DEMO_ASSERT(!inserted2);
    DEMO_ASSERT(cfg["port"] == "9090");

    // extract: remove a node and own it without destruction. The handle
    // exposes .key() and .mapped() (mutable!), letting you rename a key in
    // place -- impossible before C++17 because map keys were const.
    auto node = cfg.extract("host");
    node.key() = "hostname";
    cfg.insert(std::move(node));
    DEMO_ASSERT(cfg.count("host") == 0);
    DEMO_ASSERT(cfg.at("hostname") == "127.0.0.1");

    // merge: bulk transfer between compatible maps; conflicting keys are
    // left in the source.
    std::set<int> a{1, 2, 3};
    std::set<int> b{3, 4, 5};
    a.merge(b);
    demo::range("a after merge", a);   // {1,2,3,4,5}
    demo::range("b leftovers",   b);   // {3} -- the conflict stays in b
    DEMO_ASSERT(a.size() == 5 && b.size() == 1);
    return 0;
}
