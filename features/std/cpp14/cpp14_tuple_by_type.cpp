// description: std::get<T>(tuple) addresses a tuple element by its type instead of its index -- valid whenever exactly one element has that type.
// reference: https://en.cppreference.com/w/cpp/utility/tuple/get

#include "support/demo.hpp"
#include <string>
#include <tuple>

int main() {
    demo::title("C++14 tuple addressing by type");

    std::tuple<int, std::string, double> row{404, "not found", 0.25};

    // By type: no need to remember which slot holds what.
    DEMO_ASSERT(std::get<int>(row) == 404);
    DEMO_ASSERT(std::get<std::string>(row) == "not found");

    // It returns a reference, so assignment writes into the tuple.
    std::get<double>(row) = 0.5;
    DEMO_ASSERT(std::get<2>(row) == 0.5);

    // Both of these would be compile errors, not runtime surprises:
    //   std::get<char>(row);                        // no char element
    //   std::get<int>(std::tuple<int, int>{1, 2});  // ambiguous: two ints
    demo::text("string field", std::get<std::string>(row));
    return 0;
}
