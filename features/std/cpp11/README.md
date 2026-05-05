# C++11 examples

_Folder: `features/std/cpp11/`. 36 example(s) across 4 topic(s). Auto-generated from each file's `// gcc-test:` header — do not edit by hand. Run `python3 scripts/discover.py --emit-docs` to refresh._

## Topics

- [language](#language)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)

## language

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp11_alignof_alignas.cpp](cpp11_alignof_alignas.cpp) | c++11 | 13 | 'alignof(T)' yields T's alignment; 'alignas(N)' constrains a variable or type's alignment. |
| [cpp11_attribute_noreturn.cpp](cpp11_attribute_noreturn.cpp) | c++11 | 13 | [[noreturn]] tells the compiler a function never returns -- enables better diagnostics and dead-code elimination. |
| [cpp11_auto.cpp](cpp11_auto.cpp) | c++11 | 13 | 'auto' deduces a variable's type from its initializer; 'decltype' deduces the type of an expression without evaluating it. |
| [cpp11_constexpr.cpp](cpp11_constexpr.cpp) | c++11 | 13 | 'constexpr' allows functions and variables to be evaluated at compile time when their inputs are constant. |
| [cpp11_delete_default.cpp](cpp11_delete_default.cpp) | c++11 | 13 | '= delete' removes a special member function; '= default' asks the compiler to write the default version. |
| [cpp11_explicit_conversion.cpp](cpp11_explicit_conversion.cpp) | c++11 | 13 | 'explicit operator T()' allows static_cast<T>(obj) but blocks implicit conversion (e.g. accidental bool conversion). |
| [cpp11_initializer_list.cpp](cpp11_initializer_list.cpp) | c++11 | 13 | std::initializer_list<T> lets a function (or constructor) accept a {a, b, c, ...} brace list directly. |
| [cpp11_inline_namespace.cpp](cpp11_inline_namespace.cpp) | c++11 | 13 | An 'inline namespace' is transparently visible to its parent -- the standard mechanism for ABI versioning. |
| [cpp11_lambda.cpp](cpp11_lambda.cpp) | c++11 | 13 | Lambdas: anonymous callables with captures. [=]/[&]/[x]/[&x] cover the four common capture modes. |
| [cpp11_noexcept.cpp](cpp11_noexcept.cpp) | c++11 | 13 | 'noexcept' is a specifier (promises a function does not throw) and an operator (asks at compile time whether an expression can throw). |
| [cpp11_nullptr.cpp](cpp11_nullptr.cpp) | c++11 | 13 | 'nullptr' has its own type std::nullptr_t and isn't ambiguous between integer and pointer overloads. |
| [cpp11_override_final.cpp](cpp11_override_final.cpp) | c++11 | 13 | 'override' makes the compiler verify that a method really overrides a virtual; 'final' forbids further overriding or inheritance. |
| [cpp11_range_for.cpp](cpp11_range_for.cpp) | c++11 | 13 | Range-based for: 'for (auto& x : container)' walks anything with begin()/end() (containers, arrays, initializer_lists). |
| [cpp11_raw_string.cpp](cpp11_raw_string.cpp) | c++11 | 13 | Raw string literal R"delim(...)delim" disables backslash escapes; great for regex, paths, embedded JSON. |
| [cpp11_rvalue_move.cpp](cpp11_rvalue_move.cpp) | c++11 | 13 | Rvalue references && + std::move enable transferring resources without copying. std::forward preserves value category in templates. |
| [cpp11_scoped_enum.cpp](cpp11_scoped_enum.cpp) | c++11 | 13 | 'enum class' (scoped enums) don't leak names into the surrounding scope and don't implicitly convert to int. |
| [cpp11_static_assert.cpp](cpp11_static_assert.cpp) | c++11 | 13 | static_assert(cond, "msg") is a compile-time assertion; in C++17+ the message is optional. |
| [cpp11_trailing_return.cpp](cpp11_trailing_return.cpp) | c++11 | 13 | Trailing return type 'auto f(...) -> T' enables decltype-based return types that need parameters in scope. |
| [cpp11_uniform_init.cpp](cpp11_uniform_init.cpp) | c++11 | 13 | Brace-init {} works uniformly for aggregates, containers, and built-ins; rejects narrowing conversions. |
| [cpp11_user_defined_literal.cpp](cpp11_user_defined_literal.cpp) | c++11 | 13 | User-defined literals: 'value_suffix' calls operator"" _suffix(value). User suffixes must start with an underscore. |

## stl

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp11_array.cpp](cpp11_array.cpp) | c++11 | 13 | std::array<T,N> is a fixed-size, value-semantic, container-like wrapper around a C array; size in the type, no decay to pointer. |
| [cpp11_bind.cpp](cpp11_bind.cpp) | c++11 | 13 | std::bind partially applies arguments; placeholders _1, _2, ... thread remaining args. std::ref/cref pass references through copies. |
| [cpp11_chrono.cpp](cpp11_chrono.cpp) | c++11 | 13 | <chrono> models durations and time_points with type-safe units; arithmetic between mismatched units is rejected. |
| [cpp11_function.cpp](cpp11_function.cpp) | c++11 | 13 | std::function<R(Args...)> is a polymorphic wrapper for any callable matching the signature; type-erases the actual type. |
| [cpp11_random.cpp](cpp11_random.cpp) | c++11 | 13 | <random>: an engine (Mersenne, etc.) plus a distribution. Replaces srand/rand for serious code. |
| [cpp11_regex.cpp](cpp11_regex.cpp) | c++11 | 13 | <regex>: build a regex_basic / regex_match / regex_search / regex_replace pipeline. ECMAScript syntax by default. |
| [cpp11_smart_ptrs.cpp](cpp11_smart_ptrs.cpp) | c++11 | 13 | unique_ptr (sole ownership) / shared_ptr (refcounted) / weak_ptr (non-owning observer); std::make_shared is the preferred construction. |
| [cpp11_tuple.cpp](cpp11_tuple.cpp) | c++11 | 13 | std::tuple holds a heterogeneous fixed-size pack; std::tie + std::ignore destructure (C++17 added structured bindings). |
| [cpp11_unordered_map.cpp](cpp11_unordered_map.cpp) | c++11 | 13 | std::unordered_map / unordered_set are hash-based containers; O(1) average lookup, O(n) worst case. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp11_alias_template.cpp](cpp11_alias_template.cpp) | c++11 | 13 | 'using' creates type aliases that work as templates -- the modern replacement for 'typedef ... ::type' (e.g. std::add_const_t). |
| [cpp11_variadic_templates.cpp](cpp11_variadic_templates.cpp) | c++11 | 13 | Variadic templates take a parameter pack 'Args...' that you expand with '...' and recurse over. |

## threading

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp11_async_future.cpp](cpp11_async_future.cpp) | c++11 | 13 | std::async runs work and returns a std::future; std::promise + std::packaged_task are the lower-level building blocks. |
| [cpp11_atomic.cpp](cpp11_atomic.cpp) | c++11 | 13 | std::atomic<T> for lock-free (where supported) shared state; memory_order tunes synchronisation strength. |
| [cpp11_condition_variable.cpp](cpp11_condition_variable.cpp) | c++11 | 13 | std::condition_variable: classic mutex+predicate wait/notify. Always pair wait() with a predicate to handle spurious wake-ups. |
| [cpp11_mutex_lock.cpp](cpp11_mutex_lock.cpp) | c++11 | 13 | std::mutex + RAII locks (lock_guard, unique_lock); std::lock locks N mutexes deadlock-free. |
| [cpp11_thread.cpp](cpp11_thread.cpp) | c++11 | 13 | std::thread launches a thread; .join() waits, .detach() abandons. Failing to do either before destruction calls std::terminate. |
