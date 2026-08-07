# Coverage model

[coverage.yml](../coverage.yml) is the machine-readable inventory. It is
generated from every `gcc_feature_test()` registration plus the explicit gaps
in [coverage/gaps.yml](../coverage/gaps.yml), and CI fails when it is stale.

Statuses have precise meanings:

| Status | Meaning |
|--------|---------|
| `covered` | CI executes or statically proves the named behavior. |
| `negative` | Compilation must fail with the expected diagnostic. |
| `compile-only` | The API is present and compiles, but the supported runtime cannot yet link or execute the behavior; `skip_reason` explains why. |
| `expected-failure` | A changing implementation is tracked by a specific expected failure. Avoid this when a positive feature is available. |
| `partial` | Some but not all of the feature family is exercised. |
| `unsupported-by-libstdc++-16` | In scope, but unavailable in the newest supported library. |
| `out-of-scope-for-*` | Deliberately excluded, with the environmental reason recorded. |

`MIN_GCC` and `MIN_LIBSTDCXX` are separate axes. Generated indexes display
both; a library example is not advertised as usable merely because its
front-end syntax compiles.

Schema 2 adds the clang cross-check axis. Each entry carries `gcc_only`
(true when the example is inherently a GCC-ism or no mainline clang
implements the feature yet) and `min_clang` (the first clang major that
supports it, `null` when any supported clang does). Examples with
`gcc_only: false` build and run in the clang-22 CI lane against the same
pinned libstdc++ the GCC lanes use, so the two fields describe the
compiler front-end only — library availability stays with the
`min_libstdcxx` axis.

Coverage has no difficulty or course-progression axis. Entries are organized
by standard, compiler and library availability, topic, and proof status so the
inventory remains a reference rather than a sequence of lessons or exercises.

When adding a feature, prefer its SD-6 feature-test macro over a raw compiler
version check. Version gates decide which CI lane can try the example; the
feature macro proves that the intended facility is present.
