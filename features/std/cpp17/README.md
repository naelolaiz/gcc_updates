# C++17 examples

_Folder: `features/std/cpp17/`. 19 example(s) across 4 topic(s). Auto-generated from each file's `// gcc-test:` header — do not edit by hand. Run `python3 scripts/discover.py --emit-docs` to refresh._

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
| [cpp17_clamp.cpp](cpp17_clamp.cpp) | c++17 | 13 | std::clamp(v, lo, hi) returns max(lo, min(v, hi)); avoids the if/else dance. |
| [cpp17_filesystem.cpp](cpp17_filesystem.cpp) | c++17 | 13 | <filesystem> standardised path manipulation, directory iteration, and basic file ops. |
| [cpp17_invoke_apply.cpp](cpp17_invoke_apply.cpp) | c++17 | 13 | std::invoke calls anything callable (function, member, member ptr) uniformly; std::apply unpacks a tuple as args. |
| [cpp17_optional.cpp](cpp17_optional.cpp) | c++17 | 13 | std::optional<T> represents a value-or-nothing; use has_value(), value_or(), and emplace(). |
| [cpp17_parallel_algos.cpp](cpp17_parallel_algos.cpp) | c++17 | 13 | <numeric> gained reduce/transform_reduce/inclusive_scan/exclusive_scan; <execution> adds parallel policies. *(experimental)* |
| [cpp17_string_view.cpp](cpp17_string_view.cpp) | c++17 | 13 | std::string_view is a non-owning, copy-cheap view over a contiguous sequence of chars. |
| [cpp17_variant.cpp](cpp17_variant.cpp) | c++17 | 13 | std::variant<Ts...> is a type-safe tagged union; std::visit dispatches on the active alternative. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp17_ctad_basic.cpp](cpp17_ctad_basic.cpp) | c++17 | 13 | Class Template Argument Deduction (CTAD) lets you write 'std::pair p{1, 2.0};' without spelling the types. |
| [cpp17_fold_expressions.cpp](cpp17_fold_expressions.cpp) | c++17 | 13 | Fold expressions reduce parameter packs with a binary operator: (... op pack) and (init op ... op pack). |
| [cpp17_if_constexpr.cpp](cpp17_if_constexpr.cpp) | c++17 | 13 | 'if constexpr' discards the unselected branch at compile time, enabling clean template specialization. |

## threading

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp17_shared_mutex.cpp](cpp17_shared_mutex.cpp) | c++17 | 13 | std::shared_mutex (C++17) + std::shared_lock = many readers OR one writer; std::unique_lock for the writer. |
