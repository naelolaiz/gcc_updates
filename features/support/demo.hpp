#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

namespace demo {

inline void title(const std::string& text) {
    std::cout << "demo: " << text << '\n';
}

template <typename T>
void value(const std::string& label, const T& value) {
    std::cout << "  " << label << ": " << value << '\n';
}

inline void value(const std::string& label, bool value) {
    std::cout << "  " << label << ": " << (value ? "true" : "false") << '\n';
}

inline void text(const std::string& label, const std::string& value) {
    std::cout << "  " << label << ": " << value << '\n';
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
    std::cout << "]\n";
}

template <typename Range>
void range(const std::string& label, const Range& values) {
    range(label, values.begin(), values.end());
}

}  // namespace demo
