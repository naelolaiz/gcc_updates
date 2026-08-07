# C++14 examples

_Folder: `features/std/cpp14/`. 4 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [language](#language)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)

## language

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp14_binary_literals.cpp](cpp14_binary_literals.cpp) | c++14 | GCC >= 13 | covered | Binary literals (0b1010) and digit separators (1'000'000) -- bit masks readable at a glance, large constants readable without counting zeros. |
| [cpp14_generic_lambda.cpp](cpp14_generic_lambda.cpp) | c++14 | GCC >= 13 | covered | Generic lambdas: 'auto' parameters make the lambda's call operator a template -- one lambda works across ints, strings, and any pair type without spelling the types. |
| [cpp14_lambda_init_capture.cpp](cpp14_lambda_init_capture.cpp) | c++14 | GCC >= 13 | covered | Lambda init-captures: '[x = expr]' creates a closure member computed at capture time -- enabling move-capture ('[p = std::move(p)]'), which C++11 lambdas could not express. |
| [cpp14_relaxed_constexpr.cpp](cpp14_relaxed_constexpr.cpp) | c++14 | GCC >= 13 | covered | Relaxed constexpr: constexpr functions may contain loops, branches, and local mutation -- C++11's single-return-statement restriction is gone, so real algorithms run at compile time. |
| [cpp14_return_type_deduction.cpp](cpp14_return_type_deduction.cpp) | c++14 | GCC >= 13 | covered | Function return type deduction: ordinary functions (not just lambdas) can declare 'auto' and let their return statements fix the type; decltype(auto) preserves references exactly. |

## stl

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp14_cbegin_cend.cpp](cpp14_cbegin_cend.cpp) | c++14 | GCC >= 13 | covered | Free functions std::cbegin/cend, std::rbegin/rend and friends work on containers AND raw C arrays -- generic code gets const or reverse iterators without member-function assumptions. |
| [cpp14_dual_range_algorithms.cpp](cpp14_dual_range_algorithms.cpp) | c++14 | GCC >= 13 | covered | C++14 gave std::equal, std::mismatch, and std::is_permutation overloads taking BOTH ranges' ends -- different-length sequences compare correctly instead of reading past the shorter one. |
| [cpp14_exchange.cpp](cpp14_exchange.cpp) | c++14 | GCC >= 13 | covered | std::exchange(obj, new_value) writes the new value and returns the old one -- the one-liner behind move constructors ('data = std::exchange(other.data, nullptr)') and state flips. |
| [cpp14_heterogeneous_lookup.cpp](cpp14_heterogeneous_lookup.cpp) | c++14 | GCC >= 13 | covered | Heterogeneous ordered lookup: with the transparent comparator std::less<>, set/map find() compares the query against keys directly -- no temporary key object per lookup. |
| [cpp14_make_unique.cpp](cpp14_make_unique.cpp) | c++14 | GCC >= 13 | covered | std::make_unique<T>(args...) completes the C++11 smart-pointer story: no naked 'new', exception-safe as a function argument, plus an array form make_unique<T[]>(n). |
| [cpp14_quoted.cpp](cpp14_quoted.cpp) | c++14 | GCC >= 13 | covered | std::quoted(s) streams a string wrapped in quotes with embedded quotes escaped, and reads it back the same way -- round-trip strings containing spaces through any iostream. |
| [cpp14_std_literals.cpp](cpp14_std_literals.cpp) | c++14 | GCC >= 13 | covered | Standard user-defined literals: "text"s builds a std::string, 1500ms / 1.5s build std::chrono durations, 4.0i builds a std::complex -- typed constants with no constructor noise. |
| [cpp14_tuple_by_type.cpp](cpp14_tuple_by_type.cpp) | c++14 | GCC >= 13 | covered | std::get<T>(tuple) addresses a tuple element by its type instead of its index -- valid whenever exactly one element has that type. |

## templates

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp14_integer_sequence.cpp](cpp14_integer_sequence.cpp) | c++14 | GCC >= 13 | covered | std::integer_sequence<T, Is...> is a compile-time pack of integer constants; std::make_index_sequence<N> generates 0..N-1, the canonical way to expand a tuple/array into argument lists. |
| [cpp14_variable_templates.cpp](cpp14_variable_templates.cpp) | c++14 | GCC >= 13 | covered | Variable templates: a templated constant like pi<T> instantiates per type -- the mechanism behind C++17's _v trait shorthands (std::is_same_v etc.). |

## threading

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp14_shared_timed_mutex.cpp](cpp14_shared_timed_mutex.cpp) | c++14 | GCC >= 13 | covered | std::shared_timed_mutex + std::shared_lock (both C++14): many concurrent readers OR one exclusive writer, with try_lock_for deadlines; C++17's shared_mutex is this minus the timed API. |
