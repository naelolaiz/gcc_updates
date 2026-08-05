# C++26 (experimental) examples

_Folder: `features/std/cpp26/`. 3 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [language](#language)
- [stl](#stl)
- [templates](#templates)

## language

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp26_contracts_basic.cpp](cpp26_contracts_basic.cpp) | c++26 | 15 | C++26 contracts: pre/post/contract_assert. Implementation status varies; this file is allowed to fail on partial support. *(experimental)* |
| [cpp26_delete_reason.cpp](cpp26_delete_reason.cpp) | c++26 | 15 | = delete("reason") attaches an explanation to a deleted function; a caller's error message then says WHY it is deleted and what to use instead of a bare 'use of deleted function'. |
| [cpp26_pack_indexing.cpp](cpp26_pack_indexing.cpp) | c++26 | 15 | Pack indexing: pack...[I] selects the I-th element of a parameter pack directly -- no recursive helpers or tuple detours to reach a single pack element. |
| [cpp26_static_assert_messages.cpp](cpp26_static_assert_messages.cpp) | c++26 | 15 | static_assert accepts a constexpr string-like object (anything with constexpr size()/data()) as its message -- diagnostics can be computed instead of being string literals. |

## stl

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp26_saturation_arith.cpp](cpp26_saturation_arith.cpp) | c++26 | 14 | <numeric> in C++26 adds saturation arithmetic -- and shows live draft churn: GCC 14/15 ship add_sat/saturate_cast (__cpp_lib_saturation_arithmetic 202311), GCC 16 the renamed saturating_add/saturating_cast (202603). |
| [cpp26_span_at.cpp](cpp26_span_at.cpp) | c++26 | 15 | C++26 gives std::span bounds-checked element access: span.at(i) throws std::out_of_range like vector::at -- opt-in safety for the otherwise unchecked view. |
| [cpp26_text_encoding.cpp](cpp26_text_encoding.cpp) | c++26 | 15 | std::text_encoding identifies character encodings via the IANA registry: text_encoding::literal() reports the encoding of the string literals baked into this binary, environment() the locale's. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp26_reflection_basic.cpp](cpp26_reflection_basic.cpp) | c++26 | 16 | C++26 static reflection (P2996). API is still settling -- file is experimental. |
