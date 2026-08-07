# C++11 examples

_Folder: `features/std/cpp11/`. 13 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [callables](#callables)
- [diagnostics](#diagnostics)
- [essential](#essential)
- [initialization](#initialization)
- [language](#language)
- [memory](#memory)
- [ownership](#ownership)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)
- [type-deduction](#type-deduction)
- [type-safety](#type-safety)
- [value-categories](#value-categories)

## callables

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_lambda.cpp](cpp11_lambda.cpp) | c++11 | GCC >= 13 | covered | Lambdas: anonymous callables with captures. [=]/[&]/[x]/[&x] cover the four common capture modes. |

## diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_uniform_init_narrowing_error.cpp](cpp11_uniform_init_narrowing_error.cpp) | c++11 | GCC >= 13 | negative | Brace initialization rejects narrowing conversions at compile time; this negative example proves the diagnostic instead of leaving invalid code commented out. |

## essential

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_auto.cpp](cpp11_auto.cpp) | c++11 | GCC >= 13 | covered | 'auto' deduces a variable's type from its initializer; 'decltype' deduces the type of an expression without evaluating it. |
| [cpp11_lambda.cpp](cpp11_lambda.cpp) | c++11 | GCC >= 13 | covered | Lambdas: anonymous callables with captures. [=]/[&]/[x]/[&x] cover the four common capture modes. |
| [cpp11_nullptr.cpp](cpp11_nullptr.cpp) | c++11 | GCC >= 13 | covered | 'nullptr' has its own type std::nullptr_t and isn't ambiguous between integer and pointer overloads. |
| [cpp11_rvalue_move.cpp](cpp11_rvalue_move.cpp) | c++11 | GCC >= 13 | covered | Rvalue references && + std::move enable transferring resources without copying. std::forward preserves value category in templates. |
| [cpp11_scoped_enum.cpp](cpp11_scoped_enum.cpp) | c++11 | GCC >= 13 | covered | 'enum class' (scoped enums) don't leak names into the surrounding scope and don't implicitly convert to int. |
| [cpp11_smart_ptrs.cpp](cpp11_smart_ptrs.cpp) | c++11 | GCC >= 13 | covered | unique_ptr (sole ownership) / shared_ptr (refcounted) / weak_ptr (non-owning observer); std::make_shared is the preferred construction. |
| [cpp11_uniform_init.cpp](cpp11_uniform_init.cpp) | c++11 | GCC >= 13 | covered | Brace-init {} works uniformly for aggregates, containers, and built-ins; rejects narrowing conversions. |

## initialization

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_uniform_init.cpp](cpp11_uniform_init.cpp) | c++11 | GCC >= 13 | covered | Brace-init {} works uniformly for aggregates, containers, and built-ins; rejects narrowing conversions. |
| [cpp11_uniform_init_narrowing_error.cpp](cpp11_uniform_init_narrowing_error.cpp) | c++11 | GCC >= 13 | negative | Brace initialization rejects narrowing conversions at compile time; this negative example proves the diagnostic instead of leaving invalid code commented out. |

## language

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_alignof_alignas.cpp](cpp11_alignof_alignas.cpp) | c++11 | GCC >= 13 | covered | 'alignof(T)' yields T's alignment; 'alignas(N)' constrains a variable or type's alignment. |
| [cpp11_attribute_noreturn.cpp](cpp11_attribute_noreturn.cpp) | c++11 | GCC >= 13 | covered | [[noreturn]] tells the compiler a function never returns -- enables better diagnostics and dead-code elimination. |
| [cpp11_auto.cpp](cpp11_auto.cpp) | c++11 | GCC >= 13 | covered | 'auto' deduces a variable's type from its initializer; 'decltype' deduces the type of an expression without evaluating it. |
| [cpp11_constexpr.cpp](cpp11_constexpr.cpp) | c++11 | GCC >= 13 | covered | 'constexpr' allows functions and variables to be evaluated at compile time when their inputs are constant. |
| [cpp11_delete_default.cpp](cpp11_delete_default.cpp) | c++11 | GCC >= 13 | covered | '= delete' removes a special member function; '= default' asks the compiler to write the default version. |
| [cpp11_explicit_conversion.cpp](cpp11_explicit_conversion.cpp) | c++11 | GCC >= 13 | covered | 'explicit operator T()' allows static_cast<T>(obj) but blocks implicit conversion (e.g. accidental bool conversion). |
| [cpp11_initializer_list.cpp](cpp11_initializer_list.cpp) | c++11 | GCC >= 13 | covered | std::initializer_list<T> lets a function (or constructor) accept a {a, b, c, ...} brace list directly. |
| [cpp11_inline_namespace.cpp](cpp11_inline_namespace.cpp) | c++11 | GCC >= 13 | covered | An 'inline namespace' is transparently visible to its parent -- the standard mechanism for ABI versioning. |
| [cpp11_lambda.cpp](cpp11_lambda.cpp) | c++11 | GCC >= 13 | covered | Lambdas: anonymous callables with captures. [=]/[&]/[x]/[&x] cover the four common capture modes. |
| [cpp11_noexcept.cpp](cpp11_noexcept.cpp) | c++11 | GCC >= 13 | covered | 'noexcept' is a specifier (promises a function does not throw) and an operator (asks at compile time whether an expression can throw). |
| [cpp11_nullptr.cpp](cpp11_nullptr.cpp) | c++11 | GCC >= 13 | covered | 'nullptr' has its own type std::nullptr_t and isn't ambiguous between integer and pointer overloads. |
| [cpp11_override_final.cpp](cpp11_override_final.cpp) | c++11 | GCC >= 13 | covered | 'override' makes the compiler verify that a method really overrides a virtual; 'final' forbids further overriding or inheritance. |
| [cpp11_range_for.cpp](cpp11_range_for.cpp) | c++11 | GCC >= 13 | covered | Range-based for: 'for (auto& x : container)' walks anything with begin()/end() (containers, arrays, initializer_lists). |
| [cpp11_raw_string.cpp](cpp11_raw_string.cpp) | c++11 | GCC >= 13 | covered | Raw string literal R"delim(...)delim" disables backslash escapes; great for regex, paths, embedded JSON. |
| [cpp11_rvalue_move.cpp](cpp11_rvalue_move.cpp) | c++11 | GCC >= 13 | covered | Rvalue references && + std::move enable transferring resources without copying. std::forward preserves value category in templates. |
| [cpp11_scoped_enum.cpp](cpp11_scoped_enum.cpp) | c++11 | GCC >= 13 | covered | 'enum class' (scoped enums) don't leak names into the surrounding scope and don't implicitly convert to int. |
| [cpp11_static_assert.cpp](cpp11_static_assert.cpp) | c++11 | GCC >= 13 | covered | static_assert(cond, "msg") is a compile-time assertion; in C++17+ the message is optional. |
| [cpp11_trailing_return.cpp](cpp11_trailing_return.cpp) | c++11 | GCC >= 13 | covered | Trailing return type 'auto f(...) -> T' enables decltype-based return types that need parameters in scope. |
| [cpp11_uniform_init.cpp](cpp11_uniform_init.cpp) | c++11 | GCC >= 13 | covered | Brace-init {} works uniformly for aggregates, containers, and built-ins; rejects narrowing conversions. |
| [cpp11_uniform_init_narrowing_error.cpp](cpp11_uniform_init_narrowing_error.cpp) | c++11 | GCC >= 13 | negative | Brace initialization rejects narrowing conversions at compile time; this negative example proves the diagnostic instead of leaving invalid code commented out. |
| [cpp11_user_defined_literal.cpp](cpp11_user_defined_literal.cpp) | c++11 | GCC >= 13 | covered | User-defined literals: 'value_suffix' calls operator""_suffix(value). User suffixes must start with an underscore. |

## memory

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_smart_ptrs.cpp](cpp11_smart_ptrs.cpp) | c++11 | GCC >= 13 | covered | unique_ptr (sole ownership) / shared_ptr (refcounted) / weak_ptr (non-owning observer); std::make_shared is the preferred construction. |

## ownership

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_rvalue_move.cpp](cpp11_rvalue_move.cpp) | c++11 | GCC >= 13 | covered | Rvalue references && + std::move enable transferring resources without copying. std::forward preserves value category in templates. |
| [cpp11_smart_ptrs.cpp](cpp11_smart_ptrs.cpp) | c++11 | GCC >= 13 | covered | unique_ptr (sole ownership) / shared_ptr (refcounted) / weak_ptr (non-owning observer); std::make_shared is the preferred construction. |

## stl

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_algorithms_new.cpp](cpp11_algorithms_new.cpp) | c++11 | GCC >= 13 | covered | C++11 added a batch of <algorithm> conveniences: all_of/any_of/none_of, find_if_not, copy_if/copy_n, std::move (algorithm form), shuffle, is_sorted/is_partitioned, partition_point, minmax/minmax_element. |
| [cpp11_array.cpp](cpp11_array.cpp) | c++11 | GCC >= 13 | covered | std::array<T,N> is a fixed-size, value-semantic, container-like wrapper around a C array; size in the type, no decay to pointer. |
| [cpp11_bind.cpp](cpp11_bind.cpp) | c++11 | GCC >= 13 | covered | std::bind partially applies arguments; placeholders _1, _2, ... thread remaining args. std::ref/cref pass references through copies. |
| [cpp11_chrono.cpp](cpp11_chrono.cpp) | c++11 | GCC >= 13 | covered | <chrono> models durations and time_points with type-safe units; arithmetic between mismatched units is rejected. |
| [cpp11_emplace.cpp](cpp11_emplace.cpp) | c++11 | GCC >= 13 | covered | emplace_back / emplace forward arguments and construct the element in-place inside the container, avoiding the temporary that push_back requires. std::piecewise_construct lets std::pair / std::map values be constructed in two halves separately. |
| [cpp11_exception_ptr.cpp](cpp11_exception_ptr.cpp) | c++11 | GCC >= 13 | covered | std::exception_ptr captures an in-flight exception so it can be transported across threads / async boundaries; std::nested_exception lets a handler chain a new exception while preserving the original. |
| [cpp11_forward_list.cpp](cpp11_forward_list.cpp) | c++11 | GCC >= 13 | covered | std::forward_list is a singly-linked list -- O(1) splice/insert_after, no .size(), only forward iteration. Use when you really need a singly-linked list and not a vector or list. |
| [cpp11_function.cpp](cpp11_function.cpp) | c++11 | GCC >= 13 | covered | std::function<R(Args...)> is a polymorphic wrapper for any callable matching the signature; type-erases the actual type. |
| [cpp11_hash.cpp](cpp11_hash.cpp) | c++11 | GCC >= 13 | covered | std::hash<T> is the customisation point that makes T usable as a key in unordered_map / unordered_set. Specialising it teaches the standard hash containers about your type. |
| [cpp11_iota.cpp](cpp11_iota.cpp) | c++11 | GCC >= 13 | covered | std::iota fills a range with sequential, incrementing values starting at a given seed -- the standard "0..N-1" generator. |
| [cpp11_iterator_helpers.cpp](cpp11_iterator_helpers.cpp) | c++11 | GCC >= 13 | covered | C++11 added free std::begin/std::end (work on raw arrays AND containers), std::next/std::prev (advance an iterator by N without mutation), and std::move_iterator (turns *it into std::move(*it) for moving sequences). |
| [cpp11_random.cpp](cpp11_random.cpp) | c++11 | GCC >= 13 | covered | <random>: an engine (Mersenne, etc.) plus a distribution. Replaces srand/rand for serious code. |
| [cpp11_reference_wrapper.cpp](cpp11_reference_wrapper.cpp) | c++11 | GCC >= 13 | covered | std::reference_wrapper<T> = an assignable, copyable handle that *behaves like* a reference. std::ref / std::cref construct it; how you put 'real references' into a std::vector or pass them through std::bind/std::thread without decay-to-copy. |
| [cpp11_regex.cpp](cpp11_regex.cpp) | c++11 | GCC >= 13 | covered | <regex>: build a regex_basic / regex_match / regex_search / regex_replace pipeline. ECMAScript syntax by default. |
| [cpp11_smart_ptrs.cpp](cpp11_smart_ptrs.cpp) | c++11 | GCC >= 13 | covered | unique_ptr (sole ownership) / shared_ptr (refcounted) / weak_ptr (non-owning observer); std::make_shared is the preferred construction. |
| [cpp11_system_error.cpp](cpp11_system_error.cpp) | c++11 | GCC >= 13 | covered | <system_error> standardises error reporting -- error_code is a (value, category) pair; std::system_error is the matching exception. Used by <filesystem>, <future>, networking, etc. |
| [cpp11_tuple.cpp](cpp11_tuple.cpp) | c++11 | GCC >= 13 | covered | std::tuple holds a heterogeneous fixed-size pack; std::tie + std::ignore destructure (C++17 added structured bindings). |
| [cpp11_unordered_map.cpp](cpp11_unordered_map.cpp) | c++11 | GCC >= 13 | covered | std::unordered_map / unordered_set are hash-based containers; O(1) average lookup, O(n) worst case. |

## templates

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_alias_template.cpp](cpp11_alias_template.cpp) | c++11 | GCC >= 13 | covered | 'using' creates type aliases that work as templates -- the modern replacement for 'typedef ... ::type' (e.g. std::add_const_t). |
| [cpp11_ratio.cpp](cpp11_ratio.cpp) | c++11 | GCC >= 13 | covered | std::ratio<N, D> is a compile-time rational number; <ratio> exposes ratio_add/subtract/multiply/divide and ratio_equal/less for arithmetic on types. |
| [cpp11_type_traits.cpp](cpp11_type_traits.cpp) | c++11 | GCC >= 13 | covered | <type_traits> is the C++11 metaprogramming toolbox -- compile-time type queries (is_*) and type transformations (remove_*, decay, conditional, enable_if). |
| [cpp11_variadic_templates.cpp](cpp11_variadic_templates.cpp) | c++11 | GCC >= 13 | covered | Variadic templates take a parameter pack 'Args...' that you expand with '...' and recurse over. |

## threading

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_async_future.cpp](cpp11_async_future.cpp) | c++11 | GCC >= 13 | covered | std::async runs work and returns a std::future; std::promise + std::packaged_task are the lower-level building blocks. |
| [cpp11_atomic.cpp](cpp11_atomic.cpp) | c++11 | GCC >= 13 | covered | std::atomic<T> for lock-free (where supported) shared state; memory_order tunes synchronisation strength. |
| [cpp11_condition_variable.cpp](cpp11_condition_variable.cpp) | c++11 | GCC >= 13 | covered | std::condition_variable: classic mutex+predicate wait/notify. Always pair wait() with a predicate to handle spurious wake-ups. |
| [cpp11_mutex_lock.cpp](cpp11_mutex_lock.cpp) | c++11 | GCC >= 13 | covered | std::mutex + RAII locks (lock_guard, unique_lock); std::lock locks N mutexes deadlock-free. |
| [cpp11_thread.cpp](cpp11_thread.cpp) | c++11 | GCC >= 13 | covered | std::thread launches a thread; .join() waits, .detach() abandons. Failing to do either before destruction calls std::terminate. |
| [cpp11_thread_local.cpp](cpp11_thread_local.cpp) | c++11 | GCC >= 13 | covered | thread_local gives each thread its own copy of a static-duration variable, initialised on that thread's first use -- per-thread counters and caches without locks. |

## type-deduction

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_auto.cpp](cpp11_auto.cpp) | c++11 | GCC >= 13 | covered | 'auto' deduces a variable's type from its initializer; 'decltype' deduces the type of an expression without evaluating it. |

## type-safety

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_nullptr.cpp](cpp11_nullptr.cpp) | c++11 | GCC >= 13 | covered | 'nullptr' has its own type std::nullptr_t and isn't ambiguous between integer and pointer overloads. |
| [cpp11_scoped_enum.cpp](cpp11_scoped_enum.cpp) | c++11 | GCC >= 13 | covered | 'enum class' (scoped enums) don't leak names into the surrounding scope and don't implicitly convert to int. |

## value-categories

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp11_rvalue_move.cpp](cpp11_rvalue_move.cpp) | c++11 | GCC >= 13 | covered | Rvalue references && + std::move enable transferring resources without copying. std::forward preserves value category in templates. |
