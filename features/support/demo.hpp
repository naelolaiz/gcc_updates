#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

namespace demo {

inline void title(const std::string& text) {
    std::cout << "demo: " << text << std::endl;
}

template <typename T>
void value(const std::string& label, const T& value) {
    std::cout << "  " << label << ": " << value << std::endl;
}

inline void value(const std::string& label, bool value) {
    std::cout << "  " << label << ": "
              << (value ? "true" : "false") << std::endl;
}

inline void text(const std::string& label, const std::string& value) {
    std::cout << "  " << label << ": " << value << std::endl;
}

inline void check(const std::string& expression, bool ok) {
    std::cout << "  check: " << expression << " => "
              << (ok ? "PASS" : "FAIL") << std::endl;
}

template <typename A, typename B>
void print_item(const std::pair<A, B>& item) {
    std::cout << '(' << item.first << ", " << item.second << ')';
}

template <typename T>
void print_item(const T& item) {
    std::cout << item;
}

template <typename It>
void range(const std::string& label, It first, It last) {
    std::cout << "  " << label << ": [";
    bool need_comma = false;
    for (; first != last; ++first) {
        if (need_comma) {
            std::cout << ", ";
        }
        print_item(*first);
        need_comma = true;
    }
    std::cout << "]" << std::endl;
}

template <typename Range>
void range(const std::string& label, const Range& values) {
    range(label, values.begin(), values.end());
}

}  // namespace demo

#define DEMO_ASSERT(expr) \
    do { \
        const bool demo_assert_ok = static_cast<bool>(expr); \
        ::demo::check(#expr, demo_assert_ok); \
        assert(demo_assert_ok); \
    } while (false)
