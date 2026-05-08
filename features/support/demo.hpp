#pragma once

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>

namespace demo {

namespace detail {

// Decide once whether to emit ANSI escapes. Order matches scripts/discover.py:
// NO_COLOR wins, then FORCE_COLOR (set by discover.py when its own gate is on),
// then GITHUB_ACTIONS (CI captures stdout so isatty would be false), then
// fall back to a real terminal check.
inline bool color_enabled() {
    static const bool enabled = []() -> bool {
        if (const char* nc = std::getenv("NO_COLOR")) {
            if (nc[0] != '\0') return false;
        }
        if (const char* fc = std::getenv("FORCE_COLOR")) {
            if (fc[0] != '\0') return true;
        }
        if (const char* gha = std::getenv("GITHUB_ACTIONS")) {
            if (std::strcmp(gha, "true") == 0) return true;
        }
        return ::isatty(::fileno(stdout)) != 0;
    }();
    return enabled;
}

inline const char* code(const char* esc) {
    return color_enabled() ? esc : "";
}

}  // namespace detail

// Bright bold cyan / red render visibly stronger than the bold-on-standard
// form (\033[1;36m) in GitHub Actions' log viewer.
inline void title(const std::string& text) {
    std::cout << detail::code("\033[1;96m") << "\xe2\x96\xb6 demo: " << text
              << detail::code("\033[0m") << std::endl;
}

template <typename T>
void value(const std::string& label, const T& value) {
    std::cout << "  " << detail::code("\033[36m") << label << ":"
              << detail::code("\033[0m") << " " << value << std::endl;
}

inline void value(const std::string& label, bool value) {
    std::cout << "  " << detail::code("\033[36m") << label << ":"
              << detail::code("\033[0m") << " "
              << (value ? "true" : "false") << std::endl;
}

inline void text(const std::string& label, const std::string& value) {
    std::cout << "  " << detail::code("\033[36m") << label << ":"
              << detail::code("\033[0m") << " " << value << std::endl;
}

inline void check(const std::string& expression, bool ok) {
    std::cout << "  " << detail::code("\033[36m") << "check:"
              << detail::code("\033[0m") << " " << expression << " => "
              << detail::code(ok ? "\033[1;92m" : "\033[1;91m")
              << (ok ? "PASS" : "FAIL")
              << detail::code("\033[0m") << std::endl;
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
    std::cout << "  " << detail::code("\033[36m") << label << ":"
              << detail::code("\033[0m") << " [";
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
