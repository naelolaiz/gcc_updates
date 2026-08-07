# C++26 (experimental) examples

_Folder: `features/std/cpp26/`. 21 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [callables](#callables)
- [compile-time](#compile-time)
- [containers](#containers)
- [contracts](#contracts)
- [diagnostics](#diagnostics)
- [error-handling](#error-handling)
- [language](#language)
- [lifetimes](#lifetimes)
- [memory](#memory)
- [numerics](#numerics)
- [ownership](#ownership)
- [performance](#performance)
- [random](#random)
- [reflection](#reflection)
- [safety](#safety)
- [simd](#simd)
- [stl](#stl)
- [templates](#templates)
- [tooling](#tooling)
- [value-types](#value-types)
- [views](#views)

## callables

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_function_wrappers.cpp](cpp26_function_wrappers.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::copyable_function owns a copyable type-erased callable while std::function_ref is a lightweight non-owning view of a callable. |

## compile-time

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_constexpr_exceptions.cpp](cpp26_constexpr_exceptions.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 permits exceptions during constant evaluation when the exception and its copies are destroyed before evaluation completes. |
| [cpp26_expansion_statements.cpp](cpp26_expansion_statements.cpp) | c++26 | GCC >= 16 (GCC only) | covered | Expansion statements use template for to instantiate a statement once per compile-time element, preserving each element's distinct type or value. |
| [cpp26_pack_indexing.cpp](cpp26_pack_indexing.cpp) | c++26 | GCC >= 15 | covered | Pack indexing: pack...[I] selects the I-th element of a parameter pack directly -- no recursive helpers or tuple detours to reach a single pack element. |
| [cpp26_reflection_basic.cpp](cpp26_reflection_basic.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 static reflection inspects a type's members at compile time and synthesizes a projected array without hand-written field enumeration. |
| [cpp26_structured_binding_pack.cpp](cpp26_structured_binding_pack.cpp) | c++26 | GCC >= 16 | covered | A structured binding can introduce a pack, turning a tuple-like object into named pack elements without std::apply or index_sequence. |

## containers

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_inplace_vector.cpp](cpp26_inplace_vector.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::inplace_vector is a vector-like contiguous container whose maximum capacity and storage are part of the object, so growth never allocates. |
| [cpp26_submdspan.cpp](cpp26_submdspan.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::submdspan creates a lower-dimensional or sliced mdspan while preserving the mapping and accessor needed to view the original storage. |

## contracts

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_contracts_basic.cpp](cpp26_contracts_basic.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 contracts attach checked preconditions and postconditions to a function; GCC 16 implements the adopted P2900 syntax. |

## diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_contracts_basic.cpp](cpp26_contracts_basic.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 contracts attach checked preconditions and postconditions to a function; GCC 16 implements the adopted P2900 syntax. |
| [cpp26_debugging.cpp](cpp26_debugging.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | compile-only | <debugging> provides a portable query for debugger presence and standardized breakpoint operations. |
| [cpp26_delete_reason.cpp](cpp26_delete_reason.cpp) | c++26 | GCC >= 15 | covered | = delete("reason") attaches an explanation to a deleted function; a caller's error message then says WHY it is deleted and what to use instead of a bare 'use of deleted function'. |
| [cpp26_delete_reason_diagnostic.cpp](cpp26_delete_reason_diagnostic.cpp) | c++26 | GCC >= 15 | negative | Calling a function deleted with a reason must fail and surface the author-provided migration guidance in the compiler diagnostic. |
| [cpp26_uninitialized_diagnostic.cpp](cpp26_uninitialized_diagnostic.cpp) | c++26 | GCC >= 16 | negative | C++26 classifies an ordinary uninitialized read as erroneous behavior; GCC diagnoses this example and CI requires that diagnostic. |

## error-handling

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_constexpr_exceptions.cpp](cpp26_constexpr_exceptions.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 permits exceptions during constant evaluation when the exception and its copies are destroyed before evaluation completes. |
| [cpp26_optional_ref.cpp](cpp26_optional_ref.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::optional<T&> represents an optional non-owning reference and writes through to the original object when engaged. |

## language

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_constexpr_exceptions.cpp](cpp26_constexpr_exceptions.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 permits exceptions during constant evaluation when the exception and its copies are destroyed before evaluation completes. |
| [cpp26_contracts_basic.cpp](cpp26_contracts_basic.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 contracts attach checked preconditions and postconditions to a function; GCC 16 implements the adopted P2900 syntax. |
| [cpp26_delete_reason.cpp](cpp26_delete_reason.cpp) | c++26 | GCC >= 15 | covered | = delete("reason") attaches an explanation to a deleted function; a caller's error message then says WHY it is deleted and what to use instead of a bare 'use of deleted function'. |
| [cpp26_delete_reason_diagnostic.cpp](cpp26_delete_reason_diagnostic.cpp) | c++26 | GCC >= 15 | negative | Calling a function deleted with a reason must fail and surface the author-provided migration guidance in the compiler diagnostic. |
| [cpp26_expansion_statements.cpp](cpp26_expansion_statements.cpp) | c++26 | GCC >= 16 (GCC only) | covered | Expansion statements use template for to instantiate a statement once per compile-time element, preserving each element's distinct type or value. |
| [cpp26_pack_indexing.cpp](cpp26_pack_indexing.cpp) | c++26 | GCC >= 15 | covered | Pack indexing: pack...[I] selects the I-th element of a parameter pack directly -- no recursive helpers or tuple detours to reach a single pack element. |
| [cpp26_static_assert_messages.cpp](cpp26_static_assert_messages.cpp) | c++26 | GCC >= 15 | covered | static_assert accepts a constexpr string-like object (anything with constexpr size()/data()) as its message -- diagnostics can be computed instead of being string literals. |
| [cpp26_structured_binding_pack.cpp](cpp26_structured_binding_pack.cpp) | c++26 | GCC >= 16 | covered | A structured binding can introduce a pack, turning a tuple-like object into named pack elements without std::apply or index_sequence. |
| [cpp26_uninitialized_diagnostic.cpp](cpp26_uninitialized_diagnostic.cpp) | c++26 | GCC >= 16 | negative | C++26 classifies an ordinary uninitialized read as erroneous behavior; GCC diagnoses this example and CI requires that diagnostic. |

## lifetimes

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_optional_ref.cpp](cpp26_optional_ref.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::optional<T&> represents an optional non-owning reference and writes through to the original object when engaged. |

## memory

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_indirect_polymorphic.cpp](cpp26_indirect_polymorphic.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::indirect gives heap-backed values deep-copy semantics; std::polymorphic extends that model to copyable polymorphic class hierarchies. |
| [cpp26_inplace_vector.cpp](cpp26_inplace_vector.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::inplace_vector is a vector-like contiguous container whose maximum capacity and storage are part of the object, so growth never allocates. |

## numerics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_philox_engine.cpp](cpp26_philox_engine.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::philox4x32 is a counter-based random engine with reproducible streams and inexpensive independent subsequences for parallel workloads. |
| [cpp26_simd.cpp](cpp26_simd.cpp) | c++26 | GCC >= 16 (GCC only); libstdc++ >= 16 | covered | std::simd::vec expresses element-wise arithmetic over a fixed number of lanes and lets the implementation select vector instructions. |
| [cpp26_submdspan.cpp](cpp26_submdspan.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::submdspan creates a lower-dimensional or sliced mdspan while preserving the mapping and accessor needed to view the original storage. |

## ownership

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_function_wrappers.cpp](cpp26_function_wrappers.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::copyable_function owns a copyable type-erased callable while std::function_ref is a lightweight non-owning view of a callable. |
| [cpp26_indirect_polymorphic.cpp](cpp26_indirect_polymorphic.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::indirect gives heap-backed values deep-copy semantics; std::polymorphic extends that model to copyable polymorphic class hierarchies. |

## performance

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_simd.cpp](cpp26_simd.cpp) | c++26 | GCC >= 16 (GCC only); libstdc++ >= 16 | covered | std::simd::vec expresses element-wise arithmetic over a fixed number of lanes and lets the implementation select vector instructions. |

## random

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_philox_engine.cpp](cpp26_philox_engine.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::philox4x32 is a counter-based random engine with reproducible streams and inexpensive independent subsequences for parallel workloads. |

## reflection

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_reflection_basic.cpp](cpp26_reflection_basic.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 static reflection inspects a type's members at compile time and synthesizes a projected array without hand-written field enumeration. |

## safety

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_uninitialized_diagnostic.cpp](cpp26_uninitialized_diagnostic.cpp) | c++26 | GCC >= 16 | negative | C++26 classifies an ordinary uninitialized read as erroneous behavior; GCC diagnoses this example and CI requires that diagnostic. |

## simd

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_simd.cpp](cpp26_simd.cpp) | c++26 | GCC >= 16 (GCC only); libstdc++ >= 16 | covered | std::simd::vec expresses element-wise arithmetic over a fixed number of lanes and lets the implementation select vector instructions. |

## stl

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_debugging.cpp](cpp26_debugging.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | compile-only | <debugging> provides a portable query for debugger presence and standardized breakpoint operations. |
| [cpp26_function_wrappers.cpp](cpp26_function_wrappers.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::copyable_function owns a copyable type-erased callable while std::function_ref is a lightweight non-owning view of a callable. |
| [cpp26_indirect_polymorphic.cpp](cpp26_indirect_polymorphic.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::indirect gives heap-backed values deep-copy semantics; std::polymorphic extends that model to copyable polymorphic class hierarchies. |
| [cpp26_inplace_vector.cpp](cpp26_inplace_vector.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::inplace_vector is a vector-like contiguous container whose maximum capacity and storage are part of the object, so growth never allocates. |
| [cpp26_optional_ref.cpp](cpp26_optional_ref.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::optional<T&> represents an optional non-owning reference and writes through to the original object when engaged. |
| [cpp26_philox_engine.cpp](cpp26_philox_engine.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::philox4x32 is a counter-based random engine with reproducible streams and inexpensive independent subsequences for parallel workloads. |
| [cpp26_saturation_arith.cpp](cpp26_saturation_arith.cpp) | c++26 | GCC >= 14 | covered | <numeric> in C++26 adds saturation arithmetic -- and shows live draft churn: GCC 14/15 ship add_sat/saturate_cast (__cpp_lib_saturation_arithmetic 202311), GCC 16 the renamed saturating_add/saturating_cast (202603). |
| [cpp26_simd.cpp](cpp26_simd.cpp) | c++26 | GCC >= 16 (GCC only); libstdc++ >= 16 | covered | std::simd::vec expresses element-wise arithmetic over a fixed number of lanes and lets the implementation select vector instructions. |
| [cpp26_span_at.cpp](cpp26_span_at.cpp) | c++26 | GCC >= 15 | covered | C++26 gives std::span bounds-checked element access: span.at(i) throws std::out_of_range like vector::at -- opt-in safety for the otherwise unchecked view. |
| [cpp26_submdspan.cpp](cpp26_submdspan.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::submdspan creates a lower-dimensional or sliced mdspan while preserving the mapping and accessor needed to view the original storage. |
| [cpp26_text_encoding.cpp](cpp26_text_encoding.cpp) | c++26 | GCC >= 15 | covered | std::text_encoding identifies character encodings via the IANA registry: text_encoding::literal() reports the encoding of the string literals baked into this binary, environment() the locale's. |

## templates

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_expansion_statements.cpp](cpp26_expansion_statements.cpp) | c++26 | GCC >= 16 (GCC only) | covered | Expansion statements use template for to instantiate a statement once per compile-time element, preserving each element's distinct type or value. |
| [cpp26_pack_indexing.cpp](cpp26_pack_indexing.cpp) | c++26 | GCC >= 15 | covered | Pack indexing: pack...[I] selects the I-th element of a parameter pack directly -- no recursive helpers or tuple detours to reach a single pack element. |
| [cpp26_reflection_basic.cpp](cpp26_reflection_basic.cpp) | c++26 | GCC >= 16 (GCC only) | covered | C++26 static reflection inspects a type's members at compile time and synthesizes a projected array without hand-written field enumeration. |
| [cpp26_structured_binding_pack.cpp](cpp26_structured_binding_pack.cpp) | c++26 | GCC >= 16 | covered | A structured binding can introduce a pack, turning a tuple-like object into named pack elements without std::apply or index_sequence. |

## tooling

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_debugging.cpp](cpp26_debugging.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | compile-only | <debugging> provides a portable query for debugger presence and standardized breakpoint operations. |

## value-types

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_indirect_polymorphic.cpp](cpp26_indirect_polymorphic.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::indirect gives heap-backed values deep-copy semantics; std::polymorphic extends that model to copyable polymorphic class hierarchies. |
| [cpp26_optional_ref.cpp](cpp26_optional_ref.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::optional<T&> represents an optional non-owning reference and writes through to the original object when engaged. |

## views

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp26_submdspan.cpp](cpp26_submdspan.cpp) | c++26 | GCC >= 16; libstdc++ >= 16 | covered | std::submdspan creates a lower-dimensional or sliced mdspan while preserving the mapping and accessor needed to view the original storage. |
