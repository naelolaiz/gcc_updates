# C++17 examples

_Folder: `features/std/cpp17/`. 4 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [language](#language)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)

## language

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp17_attributes.cpp](cpp17_attributes.cpp) | c++17 | 13 | C++17 standardised three useful attributes: [[nodiscard]], [[maybe_unused]], [[fallthrough]]. |
| [cpp17_constexpr_lambda.cpp](cpp17_constexpr_lambda.cpp) | c++17 | 13 | Lambdas can be constexpr; their call operator participates in constant evaluation. |
| [cpp17_init_statement.cpp](cpp17_init_statement.cpp) | c++17 | 13 | if/switch can carry an init-statement: 'if (auto x = expr; cond)'. Locals stay scoped. |
| [cpp17_inline_variables.cpp](cpp17_inline_variables.cpp) | c++17 | 13 | 'inline' variables can be defined in a header without the ODR violation; perfect for header-only constants. |
| [cpp17_nested_namespace.cpp](cpp17_nested_namespace.cpp) | c++17 | 13 | Nested namespace declarations: 'namespace a::b::c { ... }' replaces three lines of nesting. |
| [cpp17_structured_bindings.cpp](cpp17_structured_bindings.cpp) | c++17 | 13 | Structured bindings: 'auto [a, b, c] = expr;' destructures arrays, tuples/pairs, and aggregates. |

## stl

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp17_any.cpp](cpp17_any.cpp) | c++17 | 13 | std::any is a heterogenous box that holds any copyable type; std::any_cast extracts. |
| [cpp17_byte.cpp](cpp17_byte.cpp) | c++17 | 13 | std::byte is an enum-class-shaped distinct byte type -- byte ops without unsigned char's arithmetic baggage. |
| [cpp17_charconv.cpp](cpp17_charconv.cpp) | c++17 | 13 | <charconv>::to_chars / from_chars are locale-free, allocation-free integer (and floating, since libstdc++ 11) conversion -- the fast path for parsing and formatting numbers vs std::stoi / sprintf. |
| [cpp17_clamp.cpp](cpp17_clamp.cpp) | c++17 | 13 | std::clamp(v, lo, hi) returns max(lo, min(v, hi)); avoids the if/else dance. |
| [cpp17_filesystem.cpp](cpp17_filesystem.cpp) | c++17 | 13 | <filesystem> standardised path manipulation, directory iteration, and basic file ops. |
| [cpp17_gcd_lcm_sample.cpp](cpp17_gcd_lcm_sample.cpp) | c++17 | 13 | C++17 added std::gcd / std::lcm to <numeric> and std::sample to <algorithm> -- standardised greatest-common-divisor / least-common-multiple and uniform random sampling without replacement. |
| [cpp17_invoke_apply.cpp](cpp17_invoke_apply.cpp) | c++17 | 13 | std::invoke calls anything callable (function, member, member ptr) uniformly; std::apply unpacks a tuple as args. |
| [cpp17_launder.cpp](cpp17_launder.cpp) | c++17 | 13 | std::launder<T>(p) tells the compiler "the bytes at p now hold a different T -- don't reuse cached info from the previous T at the same address". Needed when reusing storage across different types of object, especially with const/reference members. |
| [cpp17_memory_resource.cpp](cpp17_memory_resource.cpp) | c++17 | 13 | <memory_resource> introduces a polymorphic allocator (std::pmr::polymorphic_allocator) backed by a runtime memory_resource pointer; you can swap in a monotonic, pool, or custom resource without changing the container's type. |
| [cpp17_node_handles.cpp](cpp17_node_handles.cpp) | c++17 | 13 | C++17 gave map/set node handles -- extract() removes a node WITHOUT destroying it, and you can move it to another container with insert(node) or merge() (no per-element copy/move). Also try_emplace / insert_or_assign tighten map insertion semantics. |
| [cpp17_not_fn_as_const.cpp](cpp17_not_fn_as_const.cpp) | c++17 | 13 | std::not_fn(p) returns a callable that negates p (replaces the deprecated not1/not2 binders); std::as_const(x) returns a const reference, forcing the const overload of begin()/find()/etc. without copying. |
| [cpp17_optional.cpp](cpp17_optional.cpp) | c++17 | 13 | std::optional<T> represents a value-or-nothing; use has_value(), value_or(), and emplace(). |
| [cpp17_parallel_algos.cpp](cpp17_parallel_algos.cpp) | c++17 | 13 | <numeric> gained reduce/transform_reduce/inclusive_scan/exclusive_scan; <execution> adds parallel policies. |
| [cpp17_shared_ptr_array.cpp](cpp17_shared_ptr_array.cpp) | c++17 | 13 | C++17 added shared_ptr<T[]> -- shared ownership of a dynamically-allocated array, with operator[] and the correct delete[] on destruction. Pre-C++17, only unique_ptr<T[]> had this. |
| [cpp17_string_view.cpp](cpp17_string_view.cpp) | c++17 | 13 | std::string_view is a non-owning, copy-cheap view over a contiguous sequence of chars. |
| [cpp17_variant.cpp](cpp17_variant.cpp) | c++17 | 13 | std::variant<Ts...> is a type-safe tagged union; std::visit dispatches on the active alternative. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp17_ctad_basic.cpp](cpp17_ctad_basic.cpp) | c++17 | 13 | Class Template Argument Deduction (CTAD) lets you write 'std::pair p{1, 2.0};' without spelling the types. |
| [cpp17_fold_expressions.cpp](cpp17_fold_expressions.cpp) | c++17 | 13 | Fold expressions reduce parameter packs with a binary operator: (... op pack) and (init op ... op pack). |
| [cpp17_if_constexpr.cpp](cpp17_if_constexpr.cpp) | c++17 | 13 | 'if constexpr' discards the unselected branch at compile time, enabling clean template specialization. |
| [cpp17_type_trait_helpers.cpp](cpp17_type_trait_helpers.cpp) | c++17 | 13 | C++17 polished <type_traits>: _v helpers (no ::value), _t aliases (no typename ::type), std::void_t for SFINAE detection, std::conjunction/disjunction/negation for short-circuiting trait composition, and std::is_invocable / std::invoke_result. |

## threading

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp17_scoped_lock.cpp](cpp17_scoped_lock.cpp) | c++17 | 13 | std::scoped_lock<Ms...> is a variadic, deadlock-free RAII lock for any number of mutexes; one declaration replaces the C++11 std::lock + std::lock_guard pair. |
| [cpp17_shared_mutex.cpp](cpp17_shared_mutex.cpp) | c++17 | 13 | std::shared_mutex (C++17) + std::shared_lock = many readers OR one writer; std::unique_lock for the writer. |
