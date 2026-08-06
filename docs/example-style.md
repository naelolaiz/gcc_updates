# Example content and test standard

Every example is both a reference entry and an executable claim. A new example
should answer these questions, in this order:

1. **Why:** what concrete problem does the feature solve?
2. **Before:** what older idiom would otherwise be required?
3. **Core rule:** what is the smallest code that demonstrates the semantics?
4. **Boundary:** what misuse, lifetime rule, or portability constraint matters?
5. **Proof:** which runtime assertion, static assertion, diagnostic, output, or
   code-generation report proves the claim?

Use this source header:

```cpp
// description: One sentence stating the feature and the problem it solves.
// reference: A primary WG21 or GCC reference, or cppreference for API lookup.
// why: A concrete use case.
// before: The older spelling or design.
// pitfall: The most important boundary or lifetime rule.
```

The last three fields are context comments, not build metadata. Registration
metadata belongs in `CMakeLists.txt`:

```cmake
gcc_feature_test(example_name
    STD c++23  MIN_GCC 16  MIN_LIBSTDCXX 16
    TOPIC stl  TAGS error-handling vocabulary-types
    PROPOSAL P0323R12
    FEATURE_MACRO __cpp_lib_expected
    PREREQUISITES cpp17_optional)
```

CMake requires the `description` and `reference` fields for every example.
The `why`, `before`, and `pitfall` fields are optional context when they make
the reference entry clearer.

## Proof modes

- Normal examples must assert the named behavior with `DEMO_ASSERT` or
  `static_assert`.
- `EXPECT_RUN_OUTPUT` additionally proves successful stdout/stderr behavior.
- `EXPECT_ERROR` is for intentionally ill-formed examples and must match a
  stable diagnostic fragment. Set `STATUS negative`.
- `EXPECT_COMPILE_OUTPUT` proves a successful compiler report such as
  `-fanalyzer` or `-fopt-info-vec` output.
- `COMPILE_ONLY` proves header/API availability without pretending the runtime
  implementation is linkable. It requires a precise `SKIP_REASON` and should
  be upgraded to a normal test as soon as the runtime catches up.
- `MODULE_INTERFACE` proves the complete interface → importer → link → run
  sequence; compiling an importer alone is not module coverage.
- `WILL_FAIL` plus `EXPECT_OUTPUT` is reserved for deliberate runtime failures,
  such as sanitizer trips.
- Never use a preprocessor fallback that turns a required feature into a
  successful no-op. Gate it in metadata or fail clearly.

Warnings are errors by default. `ALLOW_WARNINGS` is only for a test whose
warning is itself asserted. Deliberate `WILL_FAIL` programs may warn because
their proof is the matched runtime diagnostic.
