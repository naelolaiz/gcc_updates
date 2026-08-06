# Reference paths

The generated indexes are exhaustive. These paths are intentionally short:
curated routes through the catalog, in reading order, for common starting
points.

Shortcut commands, instead of writing CTest regexes by hand:

```bash
./scripts/podman-dev.sh 16 list
./scripts/podman-dev.sh 16 show cpp23_expected
./scripts/podman-dev.sh 16 run cpp23_expected
./scripts/podman-dev.sh 16 -- -L essential
```

## Essential modern C++

This 24-example route is the recommended starting point when coming from
C++03 or another systems language.

1. Values and types: [`cpp11_auto`](../features/std/cpp11/cpp11_auto.cpp),
   [`cpp11_nullptr`](../features/std/cpp11/cpp11_nullptr.cpp),
   [`cpp11_uniform_init`](../features/std/cpp11/cpp11_uniform_init.cpp),
   [`cpp11_scoped_enum`](../features/std/cpp11/cpp11_scoped_enum.cpp).
2. Ownership and callables: [`cpp11_rvalue_move`](../features/std/cpp11/cpp11_rvalue_move.cpp),
   [`cpp11_smart_ptrs`](../features/std/cpp11/cpp11_smart_ptrs.cpp),
   [`cpp11_lambda`](../features/std/cpp11/cpp11_lambda.cpp).
3. Everyday C++17: [`cpp17_string_view`](../features/std/cpp17/cpp17_string_view.cpp),
   [`cpp17_optional`](../features/std/cpp17/cpp17_optional.cpp),
   [`cpp17_variant`](../features/std/cpp17/cpp17_variant.cpp),
   [`cpp17_structured_bindings`](../features/std/cpp17/cpp17_structured_bindings.cpp),
   [`cpp17_if_constexpr`](../features/std/cpp17/cpp17_if_constexpr.cpp).
4. C++20's new shape: [`cpp20_concepts_intro`](../features/std/cpp20/cpp20_concepts_intro.cpp),
   [`cpp20_ranges_views`](../features/std/cpp20/cpp20_ranges_views.cpp),
   [`cpp20_span`](../features/std/cpp20/cpp20_span.cpp),
   [`cpp20_format`](../features/std/cpp20/cpp20_format.cpp),
   [`cpp20_jthread`](../features/std/cpp20/cpp20_jthread.cpp),
   [`cpp20_modules_basic`](../features/std/cpp20/cpp20_modules_basic.cpp).
5. C++23's practical additions: [`cpp23_expected`](../features/std/cpp23/cpp23_expected.cpp),
   [`cpp23_print`](../features/std/cpp23/cpp23_print.cpp),
   [`cpp23_ranges_to`](../features/std/cpp23/cpp23_ranges_to.cpp),
   [`cpp23_deducing_this`](../features/std/cpp23/cpp23_deducing_this.cpp).
6. Make failures visible: [`gccext_asan_heap_use_after_free`](../features/gccext/sanitize/asan/gccext_asan_heap_use_after_free.cpp)
   and [`gccext_analyzer_null_deref`](../features/gccext/analyzer/gccext_analyzer_null_deref.cpp).

Each source file's `// why:` and `// before:` comments state the problem a
feature solves and the older idiom it replaces.

## C++17 to C++23 migration

Read these as before/after pairs:

| Older tool | Newer tool | Question to answer |
|------------|------------|--------------------|
| [`cpp11_bind`](../features/std/cpp11/cpp11_bind.cpp) | [`cpp20_bind_front`](../features/std/cpp20/cpp20_bind_front.cpp) | Which arguments remain visible at the call site? |
| [`cpp17_optional`](../features/std/cpp17/cpp17_optional.cpp) | [`cpp23_optional_monadic`](../features/std/cpp23/cpp23_optional_monadic.cpp) | Where did the nested branches go? |
| [`cpp17_string_view`](../features/std/cpp17/cpp17_string_view.cpp) | [`cpp20_span`](../features/std/cpp20/cpp20_span.cpp) | Which lifetime risks do both non-owning views share? |
| [`cpp17_if_constexpr`](../features/std/cpp17/cpp17_if_constexpr.cpp) | [`cpp20_concepts_intro`](../features/std/cpp20/cpp20_concepts_intro.cpp) | Does invalid code fail at selection or inside the body? |
| [`cpp20_is_constant_evaluated`](../features/std/cpp20/cpp20_is_constant_evaluated.cpp) | [`cpp23_if_consteval`](../features/std/cpp23/cpp23_if_consteval.cpp) | Which form communicates intent more directly? |
| [`cpp20_ranges_views`](../features/std/cpp20/cpp20_ranges_views.cpp) | [`cpp23_ranges_to`](../features/std/cpp23/cpp23_ranges_to.cpp) | When does a lazy pipeline finally own storage? |

## GCC diagnostics and safety

1. Start with [`gccext_diagnostic_pragma`](../features/gccext/pragmas/gccext_diagnostic_pragma.cpp).
2. Run the compile-fail examples with
   `./scripts/podman-dev.sh 16 -- -L diagnostics` and read the matched
   diagnostic, not only the pass/fail result.
3. Compare ASan, UBSan, TSan, and `-fanalyzer` using
   [the sanitizer guide](sanitizers.md).
4. Finish with [`gccext_autovectorize`](../features/gccext/codegen/gccext_autovectorize.cpp):
   correctness and generated-code claims are separate assertions.

## C++26 frontier

Use GCC 16. Begin with small vocabulary additions (`inplace_vector`,
`function_ref`, `optional<T&>`), then `simd`, contracts, and reflection. Check
[coverage.yml](../coverage.yml) before assuming that an absent feature was
forgotten; unsupported and intentionally out-of-scope work is recorded there.

Continue with [the evolution guide](evolution.md) or the generated
[topic index](../features/TOPICS.md) for a deeper route.
