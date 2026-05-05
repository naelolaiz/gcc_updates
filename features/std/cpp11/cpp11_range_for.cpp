// gcc-test: std=c++11 min-gcc=13 topic=language experimental=false
// description: Range-based for: 'for (auto& x : container)' walks anything with begin()/end() (containers, arrays, initializer_lists).
// reference: https://en.cppreference.com/w/cpp/language/range-for

#include <cassert>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4};

    int sum_by_value = 0;
    for (int x : v) sum_by_value += x;
    assert(sum_by_value == 10);

    // Mutate in place via reference.
    for (int& x : v) x *= 2;
    assert(v[0] == 2 && v[3] == 8);

    // Plain C array works too.
    int arr[]{10, 20, 30};
    int s = 0;
    for (auto x : arr) s += x;
    assert(s == 60);

    // Braced initializer list.
    int total = 0;
    for (auto x : {1, 2, 3, 4, 5}) total += x;
    assert(total == 15);
    return 0;
}
