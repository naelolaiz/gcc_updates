// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: std::initializer_list<T> lets a function (or constructor) accept a {a, b, c, ...} brace list directly.
// reference: https://en.cppreference.com/w/cpp/utility/initializer_list

#include "support/demo.hpp"
#include <cassert>
#include <initializer_list>
#include <vector>

class Stack {
public:
    Stack(std::initializer_list<int> il) : data_(il) {}
    int top() const { return data_.back(); }
    std::size_t size() const { return data_.size(); }
private:
    std::vector<int> data_;
};

int sum(std::initializer_list<int> il) {
    int s = 0;
    for (int v : il) s += v;
    return s;
}

int main() {
    demo::title("C++11 initializer list");
    Stack s{1, 2, 3, 4};
    DEMO_ASSERT(s.size() == 4);
    DEMO_ASSERT(s.top() == 4);

    DEMO_ASSERT(sum({10, 20, 30}) == 60);
    DEMO_ASSERT(sum({}) == 0);    // empty list -> empty initializer_list
    return 0;
}
