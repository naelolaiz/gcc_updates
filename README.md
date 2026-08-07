# gcc_updates

A structured, CI-tested workspace serving as a reference for the C++11 / C++14 /
C++17 / C++20 / C++23 / C++26 language and standard library, and tracking what
each new GCC release ships.

Every example under [features/](features/) is registered by one
`gcc_feature_test()` call in its folder's `CMakeLists.txt`; almost all are
single-file programs, with one explicit module interface/importer pair. CMake
and CTest drive the build and run; the function macro lives in
[cmake/GccFeature.cmake](cmake/GccFeature.cmake). CI runs the full suite four
times on GCC 13/14/15/16 plus three more runs on GCC 15/16 under UBSan+ASan,
TSan, and `-fanalyzer` — so correctness, runtime UB, data races, and
compile-time path analysis are all covered per push. The matching official
`gcc:N` image is used for every compiler lane, including GCC 16. A Clang 22
cross-check lane compiles and runs every portable example against the same
pinned libstdc++, proving they are standard C++ rather than GCC-isms.

## Layout

```
features/                          # example sources (usually one .cpp each)
  TOPICS.md                        # cross-bucket topic index (generated)
  support/demo.hpp                 # tiny stdout helper + DEMO_ASSERT for readable demos
  std/                             # C++ standard library + language features
    cpp11/cpp11_*.cpp              # foundation
    cpp14/cpp14_*.cpp
    cpp17/cpp17_*.cpp
    cpp20/cpp20_*.cpp
    cpp23/cpp23_*.cpp
    cpp26/cpp26_*.cpp              # C++26 positives, negative diagnostics, and compile-only probes
  gccext/                          # GCC extensions, grouped by topic
    attributes/gccext_*.cpp        # [[gnu::*]], target_clones, packed, etc.
    builtins/gccext_*.cpp          # __builtin_*, vector_size types
    codegen/gccext_*.cpp           # auto-vectorise demos, inline asm
    openmp/gccext_*.cpp            # OpenMP pragmas
    pragmas/gccext_*.cpp           # #pragma GCC diagnostic, etc.
    sanitize/                      # interaction with -fsanitize=…
      integration/gccext_*.cpp     # sanitizer-clean demos (no_sanitize, _GLIBCXX_DEBUG, [[assume]])
      asan/gccext_asan_*.cpp       # deliberate ASan trips (UAF, heap/stack OOB, double-free)
      ubsan/gccext_ubsan_*.cpp     # deliberate UBSan trips (signed overflow, null deref, shift UB)
      tsan/gccext_tsan_*.cpp       # deliberate data race
      leak/gccext_lsan_*.cpp       # deliberate leak
    analyzer/gccext_analyzer_*.cpp # compile-time -fanalyzer demos (leak/UAF/null/double-free)
  gcc/                             # release-notes smoke tests, one per version
    defaults/gccdef_*.cpp          # toolchain defaults: -std dialect, fp-contract, PIE
    gcc13/gcc13_*.cpp
    gcc14/gcc14_*.cpp
    gcc15/gcc15_*.cpp
    gcc16/gcc16_*.cpp
CMakeLists.txt                 # top-level: discovers all features/<bucket>/ subdirs
cmake/
  GccFeature.cmake             # gcc_feature_test() function and registration validation
  expect_failure.cmake         # command runner for expected failures with diagnostic regexes
  expect_command.cmake         # successful compile/runtime output assertions
  run_module_example.cmake     # ordered module interface/importer build + execution
features/<bucket>/CMakeLists.txt  # one gcc_feature_test() call per registered example
features/<bucket>/README.md     # per-bucket index (generated; CI fails if stale)
scripts/
  container-dev.sh            # local entrypoint (podman or docker + cmake + ctest)
containers/
  gcc.Containerfile           # FROM gcc:${GCC_VERSION} + cmake + libtbb-dev
  clang.Containerfile         # FROM gcc:16 + clang-22 (apt.llvm.org), same libstdc++
docs/
  reference-paths.md           # short routes through the exhaustive catalog
  example-style.md             # content and proof standard for examples
  coverage.md                  # coverage.yml schema and status meanings
  compiler-flags.md           # default and per-file flag reference
  sanitizers.md               # what -DGCC_FEATURE_SANITIZE=… adds, runtime knobs
  gcc-changelogs.md           # curated per-release notes (GCC 13 → 16)
  evolution.md                # one concept traced across standards (constexpr, lambdas, …)
  default-changes.md          # measured default-flag/dialect changes between GCC releases
.github/workflows/
  ci.yml                      # gcc 13/14/15/16 + clang-22 matrix + ubsan+asan + tsan + analyzer jobs
```

## How each file gets compiled

Every example is built with this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -Werror -O2 -pthread -Ifeatures file.cpp $EXTRA -o build/<bucket>/bin
```

`$STD` is the `STD` argument of the example's `gcc_feature_test()` call.
`$EXTRA` is its `EXTRA_LIBS` / `EXTRA_COMPILE_FLAGS` (e.g. `stdc++exp` for
`std::stacktrace`, `tbb` for parallel algorithms). Sanitizer mode swaps
`-O2` for `-O1` and adds `-fsanitize=…`; analyzer mode adds `-fanalyzer`.

[docs/compiler-flags.md](docs/compiler-flags.md) is the complete reference —
every default flag, every per-test `EXTRA_*`, and how to reproduce any
build by hand inside a container.

## Anatomy of one example

Each new example starts with the five context comments described in
[docs/example-style.md](docs/example-style.md); build metadata lives in the
folder's `CMakeLists.txt`:

```cpp
// description: std::ranges::to converts a range to a container in one expression.
// reference: https://en.cppreference.com/w/cpp/ranges/to
// why: A lazy range pipeline sometimes needs to become an owning container.
// before: Callers copied the range through an iterator-pair constructor.
// pitfall: Conversion materializes the pipeline and allocates container storage.

#include <ranges>
#include <vector>
#include <list>
#include "support/demo.hpp"

int main() {
    demo::title("C++23 ranges to");
    std::list<int> l{1, 2, 3, 4, 5};
    auto v = l | std::ranges::to<std::vector<int>>();
    DEMO_ASSERT(v.size() == 5);
    return 0;
}
```

Build metadata in `features/std/cpp23/CMakeLists.txt`:

```cmake
gcc_feature_test(cpp23_ranges_to  STD c++23  MIN_GCC 14  TOPIC ranges)
```

Required keyword args: `STD`, `MIN_GCC`, `TOPIC`. Optional: `MAX_GCC`,
`MIN_LIBSTDCXX`, `MAX_LIBSTDCXX`, `EXTRA_LIBS`, `EXTRA_COMPILE_FLAGS`,
`REQUIRES_SANITIZER`, `SKIP_SANITIZER`, `REQUIRES_ANALYZER`, `WILL_FAIL`,
`EXPECT_OUTPUT`, `EXPECT_RUN_OUTPUT`, `EXPECT_COMPILE_OUTPUT`, `COMPILE_ONLY`,
`MODULE_INTERFACE`, `EXPERIMENTAL`, `EXPECT_ERROR`, `ALLOW_WARNINGS`, `TAGS`,
`STATUS`, `ARCH`, `FEATURE_MACRO`, `PROPOSAL`, `PREREQUISITES`, and
`SKIP_REASON`. See
[cmake/GccFeature.cmake](cmake/GccFeature.cmake) for the full grammar.
Programs return `0` on success; runtime checks use `DEMO_ASSERT(...)`.

## Running locally (podman or docker)

This repo never invokes the host's `g++`/`cmake` directly. Local runs go
through [scripts/container-dev.sh](scripts/container-dev.sh), which builds a
small image from [containers/gcc.Containerfile](containers/gcc.Containerfile)
(the official `gcc:N` Docker image + `cmake` + `libtbb-dev`), then runs cmake +
ctest inside it. The build directory lives in a named volume, so it persists
across runs. Podman is the default engine; pass `engine=docker` (or set
`CONTAINER_ENGINE=docker`) to use docker instead.

```bash
./scripts/container-dev.sh 13            # build + run all GCC-13-eligible tests
./scripts/container-dev.sh 14            # same, GCC 14
./scripts/container-dev.sh 15            # same, GCC 15
./scripts/container-dev.sh 16            # same, GCC 16
./scripts/container-dev.sh clang22       # clang-22 cross-check lane

# CTest filters (everything after `--` is forwarded as-is to ctest):
./scripts/container-dev.sh 15 -- -R cpp23_         # only tests matching cpp23_
./scripts/container-dev.sh 15 -- -L threading      # only the 'threading' topic
./scripts/container-dev.sh 15 -- -j                # parallel CTest when speed matters
./scripts/container-dev.sh 15 -- -N                # list tests, don't run

# Engine / architecture selection:
./scripts/container-dev.sh 15 engine=docker        # docker instead of podman
./scripts/container-dev.sh 15 arch=arm64           # emulate linux/arm64

# Sanitizer / analyzer modes:
./scripts/container-dev.sh 15 sanitize=address,undefined
./scripts/container-dev.sh 15 sanitize=thread
./scripts/container-dev.sh 16 analyzer
./scripts/container-dev.sh 16 -- -L essential
```

The image is built once per GCC version (and per emulated arch) and cached.
`arch=` defaults to the host's native architecture; emulating another one
needs qemu binfmt support in the engine (Docker Desktop and podman machine
ship it). Architecture-specific examples (`ARCH x86` / `ARCH aarch64` in
their `gcc_feature_test()` registration, e.g. the `-mfma` and `avx2` demos
and their `*_aarch64` twins) are only registered when the compiler targets
that architecture; everything else is portable.

## Running in CI

[.github/workflows/ci.yml](.github/workflows/ci.yml) runs the compiler matrix
on both amd64 (`ubuntu-latest`) and arm64 (`ubuntu-24.04-arm`), plus sanitizer
and analyzer jobs on amd64. Every
compiler and sanitizer job runs inside the official `gcc:N` Docker image
(Debian-based, upstream gcc-N + matching upstream libstdc++-N). The same image
is what `scripts/container-dev.sh` builds locally, so behaviour is bit-identical
between local and CI for those jobs.

| Job | What it runs | Picks up |
|-----|--------------|----------|
| `gcc-{13,14,15,16} ({amd64,arm64})` | `cmake -S . -B build && ctest --test-dir build --verbose` (one row per version × architecture, in `gcc:N`) | every test whose `MIN_GCC` ≤ N and `MAX_GCC` ≥ N, minus `ARCH`-gated examples of the other architecture; GCC 16 also proves modules, contracts, reflection, and the newest library examples |
| `clang-22 ({amd64,arm64})` | `cmake -S . -B build -DCMAKE_CXX_COMPILER=clang++-22` in `gcc:16` + clang-22 from apt.llvm.org | every test not marked `GCC_ONLY` (and within its `MIN_CLANG`/libstdc++ gates), compiled with clang against the same upstream libstdc++ 16 the GCC 16 lane uses |
| `sanitize (gcc-15, ubsan + asan + lsan)` | `cmake -DGCC_FEATURE_SANITIZE=undefined,address` in `gcc:15` | every test **plus** `REQUIRES_SANITIZER` demos for {undefined, address, leak} |
| `sanitize (gcc-15, tsan)` | `cmake -DGCC_FEATURE_SANITIZE=thread` in `gcc:15` (separate; can't share with ASan) | every test plus `REQUIRES_SANITIZER thread` demos |
| `sanitize (clang-22, ubsan + asan + lsan)` | same `-DGCC_FEATURE_SANITIZE=undefined,address` configure, compiled by clang-22 against compiler-rt | the same demos proven under the upstream sanitizer runtimes |
| `sanitize (clang-22, tsan)` | `-DGCC_FEATURE_SANITIZE=thread` under clang-22 | same as the gcc tsan lane, minus `GCC_ONLY` demos |
| `analyze (gcc-16, -fanalyzer)` | `cmake -DGCC_FEATURE_ANALYZER=ON` in `gcc:16`; CTest compiles each example at `-O0` and matches its diagnostic category | `REQUIRES_ANALYZER` compile-only demos |

Each CTest run uses `--verbose` so the example output remains visible in the
log. Tests gated out by version / sanitizer / analyzer requirements are not
registered in that configure mode.

See [docs/sanitizers.md](docs/sanitizers.md) for what each sanitizer adds to
the build and how the deliberate-trip demos assert their expected report text.

### The clang cross-check lane

The `clang-22` jobs install clang from LLVM's own apt repository into the
`gcc:16` image and point it at the image's `/usr/local` GCC installation, so
clang compiles against the exact upstream libstdc++ 16 the GCC lanes use —
only the front-end changes. Registrations control the lane with three
keywords: `GCC_ONLY` excludes an example (GCC extensions, GCC-release smoke
tests, and features no mainline clang implements yet, such as reflection or
contracts), `MIN_CLANG <n>` sets a version floor, and
`EXPECT_ERROR_GCC`/`EXPECT_ERROR_CLANG` split a negative example's expected
diagnostic when the two compilers word it differently. Locally the same lane
is `./scripts/container-dev.sh clang22`
([containers/clang.Containerfile](containers/clang.Containerfile)).
`coverage.yml` records the axis per example as `gcc_only` and `min_clang`.

### libstdc++ vs g++ — separate version axes

`g++` and libstdc++ are independently versioned: they ship together in
each upstream gcc release, but the runtime libstdc++ a binary links
against can be older or newer than the one that compiled it. The
official `gcc:N` Docker images keep both at version N, which is why CI
uses them. To express a libstdc++ requirement on individual files:

- `cmake/GccFeature.cmake` probes `_GLIBCXX_RELEASE` at configure time and
  caches it in `LIBSTDCXX_RELEASE`.
- `gcc_feature_test()` accepts `MIN_LIBSTDCXX` / `MAX_LIBSTDCXX`; tests
  outside that window are not registered in that configure mode.

Some C++23 library features ship at different libstdc++ releases than
their language counterparts. Notably, per cppreference:

| Feature | First libstdc++ release |
|---------|-------------------------|
| `std::ranges::fold_*`, `find_last*`, `contains`/`contains_subrange` | 13 |
| `std::stacktrace` (linked via `-lstdc++exp`) | 14 |
| `std::mdspan`, `std::start_lifetime_as`, `std::ranges::starts_with` / `ends_with` | 16 |

The ranges algorithms in the table are declared in `<algorithm>` (not
`<ranges>`); forgetting that include is a common reason one appears missing.

#### Runtime ABI: forward-compatible, not backward-compatible

A separate concern from compile-time availability: each libstdc++
release adds versioned ELF symbols (`GLIBCXX_3.4.30`, `3.4.31`, …); the
loader records the highest tag a binary actually uses and rejects it on
a system whose libstdc++ doesn't have at least that tag.

| Build environment | Run on libstdc++ ≥ build | Run on libstdc++ < build |
|-------------------|--------------------------|--------------------------|
| libstdc++ N | ✅ works (forward-compat) | ❌ `version 'GLIBCXX_3.4.MM' not found (required by ./bin)` |

A binary built inside `gcc:15` may refuse to start on a system shipping
older libstdc++. To ship something portable:

```bash
# Bundle libstdc++ into the binary. Bigger executable; no host upgrades.
g++ -std=c++23 -O2 -static-libstdc++ -static-libgcc your_file.cpp -o your_bin

# Or: ship libstdc++.so.6 alongside the binary and point the loader at it.
g++ -std=c++23 -O2 -Wl,-rpath,'$ORIGIN' your_file.cpp -o your_bin
cp /usr/lib/x86_64-linux-gnu/libstdc++.so.6 ./   # next to your_bin

# Or: build inside a sysroot of the oldest deployment target.
```

CI itself never ships binaries outside the container that built them,
so this concern is purely informational.

## Adding a new example

1. Drop a `.cpp` under [features/](features/) in the right subfolder (a module
   example may also have one interface `.cpp` named by `MODULE_INTERFACE`):
   `features/std/cppNN/`, `features/gcc/gccNN/`, or
   `features/gccext/<topic>/`. Filename must start with the bucket prefix
   (`cpp11_`, `cpp20_`, `gcc14_`, `gccext_`, …).
2. Add the five context comments from [docs/example-style.md](docs/example-style.md):
   `description`, `reference`, `why`, `before`, and `pitfall`.
3. Write `int main()` that asserts what should hold, returns 0 on success.
4. Add a `gcc_feature_test(<name> STD c++NN MIN_GCC N TOPIC <topic>)` line
   to that folder's `CMakeLists.txt`. Mark it `GCC_ONLY` (or give it a
   `MIN_CLANG`) if the clang lane cannot build it. See
   [cmake/GccFeature.cmake](cmake/GccFeature.cmake) for the full grammar.
5. Run `./scripts/container-dev.sh <ver>` to verify locally.
6. Run `./scripts/container-dev.sh <ver> readme` to regenerate the bucket's
   `README.md` index. Every configure validates the indexes against the
   registered metadata (`-DGCC_FEATURE_README=check`, the default), so CI
   fails if this step is skipped. The index title (the H1 line) is the only
   hand-written part and is preserved across regenerations. Configure also
   checks every relative link in the repo's markdown (hand-written docs
   included), so renaming or removing an example fails fast instead of
   leaving dead links.

## Reference tracks

Start with [docs/reference-paths.md](docs/reference-paths.md). It provides a
24-example essential route, a C++17-to-C++23 migration route, a GCC safety
route, and a C++26 frontier route. The required content/test format for new
examples is documented in [docs/example-style.md](docs/example-style.md).

The exhaustive machine-readable inventory is [coverage.yml](coverage.yml);
[docs/coverage.md](docs/coverage.md) explains its statuses and how intentional
gaps are recorded.

Command shortcuts:

```bash
./scripts/container-dev.sh 16 list
./scripts/container-dev.sh 16 show cpp23_expected
./scripts/container-dev.sh 16 run cpp23_expected
./scripts/container-dev.sh 16 -- -L essential
```

## Full reference catalog

Most examples are independent and self-contained; the modules example is an
intentional interface/importer pair. If you're walking through them as a
structured tour, this is the order I'd suggest. Each
step links to the per-bucket index that lists every example for that standard,
grouped by topic. Two cross-cutting entry points complement this path:
[docs/evolution.md](docs/evolution.md) traces single concepts (constexpr,
lambdas, ranges, concurrency, …) across all standards, and
[features/TOPICS.md](features/TOPICS.md) lists every topic label with its
examples from every bucket.

### 1. Foundation (C++11)

If you only know pre-C++11, this is the unavoidable starting point. Everything
that came after assumes you have these in muscle memory.

- **Core language:** [`cpp11_auto`](features/std/cpp11/cpp11_auto.cpp),
  [`cpp11_range_for`](features/std/cpp11/cpp11_range_for.cpp),
  [`cpp11_nullptr`](features/std/cpp11/cpp11_nullptr.cpp),
  [`cpp11_scoped_enum`](features/std/cpp11/cpp11_scoped_enum.cpp),
  [`cpp11_uniform_init`](features/std/cpp11/cpp11_uniform_init.cpp),
  [`cpp11_lambda`](features/std/cpp11/cpp11_lambda.cpp),
  [`cpp11_constexpr`](features/std/cpp11/cpp11_constexpr.cpp),
  [`cpp11_noexcept`](features/std/cpp11/cpp11_noexcept.cpp),
  [`cpp11_static_assert`](features/std/cpp11/cpp11_static_assert.cpp),
  [`cpp11_override_final`](features/std/cpp11/cpp11_override_final.cpp),
  [`cpp11_delete_default`](features/std/cpp11/cpp11_delete_default.cpp),
  [`cpp11_trailing_return`](features/std/cpp11/cpp11_trailing_return.cpp),
  [`cpp11_explicit_conversion`](features/std/cpp11/cpp11_explicit_conversion.cpp).
- **Move semantics (the hardest one — read it twice):**
  [`cpp11_rvalue_move`](features/std/cpp11/cpp11_rvalue_move.cpp).
- **Templates / metaprogramming:**
  [`cpp11_variadic_templates`](features/std/cpp11/cpp11_variadic_templates.cpp),
  [`cpp11_alias_template`](features/std/cpp11/cpp11_alias_template.cpp),
  [`cpp11_type_traits`](features/std/cpp11/cpp11_type_traits.cpp),
  [`cpp11_ratio`](features/std/cpp11/cpp11_ratio.cpp).
- **STL value types:** [`cpp11_array`](features/std/cpp11/cpp11_array.cpp),
  [`cpp11_tuple`](features/std/cpp11/cpp11_tuple.cpp),
  [`cpp11_unordered_map`](features/std/cpp11/cpp11_unordered_map.cpp),
  [`cpp11_initializer_list`](features/std/cpp11/cpp11_initializer_list.cpp),
  [`cpp11_smart_ptrs`](features/std/cpp11/cpp11_smart_ptrs.cpp),
  [`cpp11_chrono`](features/std/cpp11/cpp11_chrono.cpp),
  [`cpp11_random`](features/std/cpp11/cpp11_random.cpp),
  [`cpp11_regex`](features/std/cpp11/cpp11_regex.cpp),
  [`cpp11_forward_list`](features/std/cpp11/cpp11_forward_list.cpp).
- **STL infrastructure:** [`cpp11_emplace`](features/std/cpp11/cpp11_emplace.cpp),
  [`cpp11_hash`](features/std/cpp11/cpp11_hash.cpp),
  [`cpp11_reference_wrapper`](features/std/cpp11/cpp11_reference_wrapper.cpp),
  [`cpp11_iterator_helpers`](features/std/cpp11/cpp11_iterator_helpers.cpp),
  [`cpp11_algorithms_new`](features/std/cpp11/cpp11_algorithms_new.cpp),
  [`cpp11_iota`](features/std/cpp11/cpp11_iota.cpp),
  [`cpp11_system_error`](features/std/cpp11/cpp11_system_error.cpp),
  [`cpp11_exception_ptr`](features/std/cpp11/cpp11_exception_ptr.cpp).
- **Callables:** [`cpp11_function`](features/std/cpp11/cpp11_function.cpp),
  [`cpp11_bind`](features/std/cpp11/cpp11_bind.cpp).
- **Concurrency:** [`cpp11_thread`](features/std/cpp11/cpp11_thread.cpp),
  [`cpp11_thread_local`](features/std/cpp11/cpp11_thread_local.cpp),
  [`cpp11_mutex_lock`](features/std/cpp11/cpp11_mutex_lock.cpp),
  [`cpp11_condition_variable`](features/std/cpp11/cpp11_condition_variable.cpp),
  [`cpp11_async_future`](features/std/cpp11/cpp11_async_future.cpp),
  [`cpp11_atomic`](features/std/cpp11/cpp11_atomic.cpp).
- **Niceties:** [`cpp11_raw_string`](features/std/cpp11/cpp11_raw_string.cpp),
  [`cpp11_user_defined_literal`](features/std/cpp11/cpp11_user_defined_literal.cpp),
  [`cpp11_alignof_alignas`](features/std/cpp11/cpp11_alignof_alignas.cpp),
  [`cpp11_attribute_noreturn`](features/std/cpp11/cpp11_attribute_noreturn.cpp),
  [`cpp11_inline_namespace`](features/std/cpp11/cpp11_inline_namespace.cpp).

Full index: [features/std/cpp11/README.md](features/std/cpp11/README.md).

### 1.5 The C++11 clean-up round (C++14)

A small release, but its additions are everywhere in modern code — most of
what people call "C++11 style" is actually C++11 + these.

- **Language:** [`cpp14_generic_lambda`](features/std/cpp14/cpp14_generic_lambda.cpp),
  [`cpp14_lambda_init_capture`](features/std/cpp14/cpp14_lambda_init_capture.cpp),
  [`cpp14_relaxed_constexpr`](features/std/cpp14/cpp14_relaxed_constexpr.cpp),
  [`cpp14_return_type_deduction`](features/std/cpp14/cpp14_return_type_deduction.cpp),
  [`cpp14_binary_literals`](features/std/cpp14/cpp14_binary_literals.cpp).
- **Templates / metaprogramming:**
  [`cpp14_variable_templates`](features/std/cpp14/cpp14_variable_templates.cpp),
  [`cpp14_integer_sequence`](features/std/cpp14/cpp14_integer_sequence.cpp).
- **STL:** [`cpp14_make_unique`](features/std/cpp14/cpp14_make_unique.cpp),
  [`cpp14_exchange`](features/std/cpp14/cpp14_exchange.cpp),
  [`cpp14_std_literals`](features/std/cpp14/cpp14_std_literals.cpp),
  [`cpp14_tuple_by_type`](features/std/cpp14/cpp14_tuple_by_type.cpp),
  [`cpp14_heterogeneous_lookup`](features/std/cpp14/cpp14_heterogeneous_lookup.cpp),
  [`cpp14_quoted`](features/std/cpp14/cpp14_quoted.cpp),
  [`cpp14_dual_range_algorithms`](features/std/cpp14/cpp14_dual_range_algorithms.cpp),
  [`cpp14_cbegin_cend`](features/std/cpp14/cpp14_cbegin_cend.cpp).
- **Concurrency:** [`cpp14_shared_timed_mutex`](features/std/cpp14/cpp14_shared_timed_mutex.cpp).

Full index: [features/std/cpp14/README.md](features/std/cpp14/README.md).

### 2. Quality-of-life (C++17)

- **STL value types:** [`cpp17_optional`](features/std/cpp17/cpp17_optional.cpp),
  [`cpp17_variant`](features/std/cpp17/cpp17_variant.cpp),
  [`cpp17_any`](features/std/cpp17/cpp17_any.cpp),
  [`cpp17_string_view`](features/std/cpp17/cpp17_string_view.cpp),
  [`cpp17_byte`](features/std/cpp17/cpp17_byte.cpp),
  [`cpp17_filesystem`](features/std/cpp17/cpp17_filesystem.cpp),
  [`cpp17_shared_ptr_array`](features/std/cpp17/cpp17_shared_ptr_array.cpp).
- **Algorithms / numerics:** [`cpp17_clamp`](features/std/cpp17/cpp17_clamp.cpp),
  [`cpp17_invoke_apply`](features/std/cpp17/cpp17_invoke_apply.cpp),
  [`cpp17_parallel_algos`](features/std/cpp17/cpp17_parallel_algos.cpp),
  [`cpp17_gcd_lcm_sample`](features/std/cpp17/cpp17_gcd_lcm_sample.cpp),
  [`cpp17_not_fn_as_const`](features/std/cpp17/cpp17_not_fn_as_const.cpp),
  [`cpp17_searchers`](features/std/cpp17/cpp17_searchers.cpp) (Boyer-Moore).
- **STL infrastructure:** [`cpp17_charconv`](features/std/cpp17/cpp17_charconv.cpp),
  [`cpp17_memory_resource`](features/std/cpp17/cpp17_memory_resource.cpp),
  [`cpp17_node_handles`](features/std/cpp17/cpp17_node_handles.cpp),
  [`cpp17_launder`](features/std/cpp17/cpp17_launder.cpp).
- **Templates / metaprogramming:**
  [`cpp17_if_constexpr`](features/std/cpp17/cpp17_if_constexpr.cpp),
  [`cpp17_fold_expressions`](features/std/cpp17/cpp17_fold_expressions.cpp),
  [`cpp17_ctad_basic`](features/std/cpp17/cpp17_ctad_basic.cpp),
  [`cpp17_type_trait_helpers`](features/std/cpp17/cpp17_type_trait_helpers.cpp).
- **Language:** [`cpp17_structured_bindings`](features/std/cpp17/cpp17_structured_bindings.cpp),
  [`cpp17_init_statement`](features/std/cpp17/cpp17_init_statement.cpp),
  [`cpp17_inline_variables`](features/std/cpp17/cpp17_inline_variables.cpp),
  [`cpp17_nested_namespace`](features/std/cpp17/cpp17_nested_namespace.cpp),
  [`cpp17_constexpr_lambda`](features/std/cpp17/cpp17_constexpr_lambda.cpp),
  [`cpp17_attributes`](features/std/cpp17/cpp17_attributes.cpp).
- **Concurrency:** [`cpp17_shared_mutex`](features/std/cpp17/cpp17_shared_mutex.cpp),
  [`cpp17_scoped_lock`](features/std/cpp17/cpp17_scoped_lock.cpp).

Full index: [features/std/cpp17/README.md](features/std/cpp17/README.md).

### 3. The big jump (C++20)

This is where the language genuinely changed shape.

- **Concepts & templates / metaprogramming:**
  [`cpp20_concepts_intro`](features/std/cpp20/cpp20_concepts_intro.cpp),
  [`cpp20_concepts_requires_expr`](features/std/cpp20/cpp20_concepts_requires_expr.cpp),
  [`cpp20_abbreviated_templates`](features/std/cpp20/cpp20_abbreviated_templates.cpp),
  [`cpp20_ctad_alias`](features/std/cpp20/cpp20_ctad_alias.cpp),
  [`cpp20_ctad_aggregates`](features/std/cpp20/cpp20_ctad_aggregates.cpp),
  [`cpp20_nttp_class`](features/std/cpp20/cpp20_nttp_class.cpp),
  [`cpp20_explicit_bool`](features/std/cpp20/cpp20_explicit_bool.cpp),
  [`cpp20_type_identity`](features/std/cpp20/cpp20_type_identity.cpp).
- **Ranges & views:** [`cpp20_ranges_algorithms`](features/std/cpp20/cpp20_ranges_algorithms.cpp),
  [`cpp20_ranges_views`](features/std/cpp20/cpp20_ranges_views.cpp),
  [`cpp20_views_keys_values`](features/std/cpp20/cpp20_views_keys_values.cpp),
  [`cpp20_views_split`](features/std/cpp20/cpp20_views_split.cpp).
- **Coroutines:** [`cpp20_coroutine_generator`](features/std/cpp20/cpp20_coroutine_generator.cpp)
  (read after concepts; this is the hardest single feature in C++20).
- **Concurrency (the *big* upgrade):** [`cpp20_jthread`](features/std/cpp20/cpp20_jthread.cpp),
  [`cpp20_stop_token`](features/std/cpp20/cpp20_stop_token.cpp),
  [`cpp20_counting_semaphore`](features/std/cpp20/cpp20_counting_semaphore.cpp),
  [`cpp20_binary_semaphore`](features/std/cpp20/cpp20_binary_semaphore.cpp),
  [`cpp20_latch`](features/std/cpp20/cpp20_latch.cpp),
  [`cpp20_barrier`](features/std/cpp20/cpp20_barrier.cpp),
  [`cpp20_atomic_wait`](features/std/cpp20/cpp20_atomic_wait.cpp),
  [`cpp20_atomic_ref`](features/std/cpp20/cpp20_atomic_ref.cpp),
  [`cpp20_atomic_shared_ptr`](features/std/cpp20/cpp20_atomic_shared_ptr.cpp),
  [`cpp20_atomic_flag_test`](features/std/cpp20/cpp20_atomic_flag_test.cpp),
  [`cpp20_syncstream`](features/std/cpp20/cpp20_syncstream.cpp).
- **Library:** [`cpp20_format`](features/std/cpp20/cpp20_format.cpp),
  [`cpp20_span`](features/std/cpp20/cpp20_span.cpp),
  [`cpp20_bit_ops`](features/std/cpp20/cpp20_bit_ops.cpp),
  [`cpp20_source_location`](features/std/cpp20/cpp20_source_location.cpp),
  [`cpp20_numbers`](features/std/cpp20/cpp20_numbers.cpp),
  [`cpp20_chrono_calendar`](features/std/cpp20/cpp20_chrono_calendar.cpp),
  [`cpp20_erase_if`](features/std/cpp20/cpp20_erase_if.cpp),
  [`cpp20_lerp_midpoint`](features/std/cpp20/cpp20_lerp_midpoint.cpp),
  [`cpp20_ssize`](features/std/cpp20/cpp20_ssize.cpp),
  [`cpp20_string_starts_ends`](features/std/cpp20/cpp20_string_starts_ends.cpp),
  [`cpp20_map_contains`](features/std/cpp20/cpp20_map_contains.cpp),
  [`cpp20_to_address`](features/std/cpp20/cpp20_to_address.cpp),
  [`cpp20_endian`](features/std/cpp20/cpp20_endian.cpp),
  [`cpp20_bind_front`](features/std/cpp20/cpp20_bind_front.cpp),
  [`cpp20_cmp_utilities`](features/std/cpp20/cpp20_cmp_utilities.cpp).
- **Language:** [`cpp20_consteval`](features/std/cpp20/cpp20_consteval.cpp),
  [`cpp20_constinit`](features/std/cpp20/cpp20_constinit.cpp),
  [`cpp20_spaceship`](features/std/cpp20/cpp20_spaceship.cpp),
  [`cpp20_designated_init`](features/std/cpp20/cpp20_designated_init.cpp),
  [`cpp20_lambdas`](features/std/cpp20/cpp20_lambdas.cpp),
  [`cpp20_using_enum`](features/std/cpp20/cpp20_using_enum.cpp),
  [`cpp20_is_constant_evaluated`](features/std/cpp20/cpp20_is_constant_evaluated.cpp).

Full index: [features/std/cpp20/README.md](features/std/cpp20/README.md).

### 4. Polishing (C++23)

Mostly about smoothing C++20's rough edges plus a few headline items.

- **The headline items:** [`cpp23_expected`](features/std/cpp23/cpp23_expected.cpp),
  [`cpp23_mdspan`](features/std/cpp23/cpp23_mdspan.cpp),
  [`cpp23_print`](features/std/cpp23/cpp23_print.cpp),
  [`cpp23_generator`](features/std/cpp23/cpp23_generator.cpp),
  [`cpp23_stacktrace`](features/std/cpp23/cpp23_stacktrace.cpp),
  [`cpp23_flat_map`](features/std/cpp23/cpp23_flat_map.cpp),
  [`cpp23_flat_set`](features/std/cpp23/cpp23_flat_set.cpp).
- **Templates / language:** [`cpp23_deducing_this`](features/std/cpp23/cpp23_deducing_this.cpp)
  (huge), [`cpp23_recursive_lambda`](features/std/cpp23/cpp23_recursive_lambda.cpp),
  [`cpp23_static_operator`](features/std/cpp23/cpp23_static_operator.cpp),
  [`cpp23_multidim_subscript`](features/std/cpp23/cpp23_multidim_subscript.cpp),
  [`cpp23_if_consteval`](features/std/cpp23/cpp23_if_consteval.cpp),
  [`cpp23_assume`](features/std/cpp23/cpp23_assume.cpp),
  [`cpp23_size_t_literal`](features/std/cpp23/cpp23_size_t_literal.cpp),
  [`cpp23_auto_decay_copy`](features/std/cpp23/cpp23_auto_decay_copy.cpp).
- **Ranges/views:** [`cpp23_ranges_to`](features/std/cpp23/cpp23_ranges_to.cpp),
  [`cpp23_ranges_zip`](features/std/cpp23/cpp23_ranges_zip.cpp),
  [`cpp23_ranges_chunk_slide`](features/std/cpp23/cpp23_ranges_chunk_slide.cpp),
  [`cpp23_views_chunk_by`](features/std/cpp23/cpp23_views_chunk_by.cpp),
  [`cpp23_ranges_enumerate`](features/std/cpp23/cpp23_ranges_enumerate.cpp),
  [`cpp23_ranges_join_with`](features/std/cpp23/cpp23_ranges_join_with.cpp),
  [`cpp23_ranges_cartesian_product`](features/std/cpp23/cpp23_ranges_cartesian_product.cpp),
  [`cpp23_ranges_adjacent`](features/std/cpp23/cpp23_ranges_adjacent.cpp),
  [`cpp23_ranges_fold`](features/std/cpp23/cpp23_ranges_fold.cpp),
  [`cpp23_ranges_starts_ends_contains`](features/std/cpp23/cpp23_ranges_starts_ends_contains.cpp),
  [`cpp23_ranges_find_last`](features/std/cpp23/cpp23_ranges_find_last.cpp),
  [`cpp23_ranges_iota_algorithm`](features/std/cpp23/cpp23_ranges_iota_algorithm.cpp),
  [`cpp23_views_repeat`](features/std/cpp23/cpp23_views_repeat.cpp),
  [`cpp23_views_stride`](features/std/cpp23/cpp23_views_stride.cpp),
  [`cpp23_views_as_const_as_rvalue`](features/std/cpp23/cpp23_views_as_const_as_rvalue.cpp).
- **Library:** [`cpp23_optional_monadic`](features/std/cpp23/cpp23_optional_monadic.cpp),
  [`cpp23_byteswap`](features/std/cpp23/cpp23_byteswap.cpp),
  [`cpp23_to_underlying`](features/std/cpp23/cpp23_to_underlying.cpp),
  [`cpp23_unreachable`](features/std/cpp23/cpp23_unreachable.cpp),
  [`cpp23_move_only_function`](features/std/cpp23/cpp23_move_only_function.cpp),
  [`cpp23_out_ptr`](features/std/cpp23/cpp23_out_ptr.cpp),
  [`cpp23_string_contains`](features/std/cpp23/cpp23_string_contains.cpp),
  [`cpp23_resize_and_overwrite`](features/std/cpp23/cpp23_resize_and_overwrite.cpp),
  [`cpp23_spanstream`](features/std/cpp23/cpp23_spanstream.cpp),
  [`cpp23_format_ranges`](features/std/cpp23/cpp23_format_ranges.cpp),
  [`cpp23_forward_like`](features/std/cpp23/cpp23_forward_like.cpp),
  [`cpp23_start_lifetime_as`](features/std/cpp23/cpp23_start_lifetime_as.cpp),
  [`cpp23_invoke_r`](features/std/cpp23/cpp23_invoke_r.cpp),
  [`cpp23_stdfloat`](features/std/cpp23/cpp23_stdfloat.cpp).

Full index: [features/std/cpp23/README.md](features/std/cpp23/README.md).

### 5. GCC-specific extensions (orthogonal to standards)

These features aren't in any C++ standard but ship with every modern GCC.
Read these alongside the standard-library bucket — `[[likely]]` is C++20 *and*
implemented via the same machinery as `__builtin_expect`, etc.

- **Attributes:** [`gccext_likely_unlikely`](features/gccext/attributes/gccext_likely_unlikely.cpp),
  [`gccext_attribute_pure_const`](features/gccext/attributes/gccext_attribute_pure_const.cpp),
  [`gccext_attribute_hot_cold`](features/gccext/attributes/gccext_attribute_hot_cold.cpp),
  [`gccext_attribute_flatten`](features/gccext/attributes/gccext_attribute_flatten.cpp),
  [`gccext_attribute_packed`](features/gccext/attributes/gccext_attribute_packed.cpp),
  [`gccext_attribute_cleanup`](features/gccext/attributes/gccext_attribute_cleanup.cpp),
  [`gccext_attribute_constructor`](features/gccext/attributes/gccext_attribute_constructor.cpp)
  (pre-main/post-main hooks),
  [`gccext_attribute_target`](features/gccext/attributes/gccext_attribute_target.cpp)
  (per-function ISA selection; x86 — AArch64 twin
  [`gccext_attribute_target_aarch64`](features/gccext/attributes/gccext_attribute_target_aarch64.cpp)),
  [`gccext_target_clones`](features/gccext/attributes/gccext_target_clones.cpp)
  (multi-version + IFUNC dispatch; x86 — AArch64 twin
  [`gccext_target_clones_aarch64`](features/gccext/attributes/gccext_target_clones_aarch64.cpp)).
- **Builtins:** [`gccext_builtin_expect`](features/gccext/builtins/gccext_builtin_expect.cpp),
  [`gccext_builtin_constant_p`](features/gccext/builtins/gccext_builtin_constant_p.cpp),
  [`gccext_builtin_assume_aligned`](features/gccext/builtins/gccext_builtin_assume_aligned.cpp),
  [`gccext_builtin_cpu_supports`](features/gccext/builtins/gccext_builtin_cpu_supports.cpp)
  (manual runtime dispatch; x86-only — AArch64 dispatches via `target_clones`),
  [`gccext_builtin_prefetch`](features/gccext/builtins/gccext_builtin_prefetch.cpp),
  [`gccext_int128`](features/gccext/builtins/gccext_int128.cpp).
- **Vectorization / SIMD:** [`gccext_vector_extensions`](features/gccext/builtins/gccext_vector_extensions.cpp)
  (`vector_size` types),
  [`gccext_autovectorize`](features/gccext/codegen/gccext_autovectorize.cpp)
  (auto-vectorisable SAXPY at `-O3`).
- **Parallelism:** [`gccext_openmp_parallel_for`](features/gccext/openmp/gccext_openmp_parallel_for.cpp).
- **Tooling:** [`gccext_diagnostic_pragma`](features/gccext/pragmas/gccext_diagnostic_pragma.cpp),
  [`gccext_inline_asm`](features/gccext/codegen/gccext_inline_asm.cpp).
- **Sanitizer interaction (clean):** [`gccext_no_sanitize_attribute`](features/gccext/sanitize/integration/gccext_no_sanitize_attribute.cpp),
  [`gccext_glibcxx_debug`](features/gccext/sanitize/integration/gccext_glibcxx_debug.cpp),
  [`gccext_assume_under_sanitize`](features/gccext/sanitize/integration/gccext_assume_under_sanitize.cpp)
  — how to opt-in, opt-out, and reason about runtime checks. Live under
  [features/gccext/sanitize/integration/](features/gccext/sanitize/integration/).
- **Sanitizer trips (deliberate UB):** `gccext_asan_*` (in
  [sanitize/asan/](features/gccext/sanitize/asan/)), `gccext_ubsan_*` (in
  [sanitize/ubsan/](features/gccext/sanitize/ubsan/)), `gccext_tsan_*` (in
  [sanitize/tsan/](features/gccext/sanitize/tsan/)), `gccext_lsan_*` (in
  [sanitize/leak/](features/gccext/sanitize/leak/)) — each one is gated on a
  matching `REQUIRES_SANITIZER` and asserts expected sanitizer report text
  instead of accepting any failure. Run with
  `./scripts/container-dev.sh 15 sanitize=undefined,address` (or
  `sanitize=thread`).
- **Static analyzer (compile-only):** [`gccext_analyzer_double_free`](features/gccext/analyzer/gccext_analyzer_double_free.cpp),
  [`gccext_analyzer_leak`](features/gccext/analyzer/gccext_analyzer_leak.cpp),
  [`gccext_analyzer_null_deref`](features/gccext/analyzer/gccext_analyzer_null_deref.cpp),
  [`gccext_analyzer_use_after_free`](features/gccext/analyzer/gccext_analyzer_use_after_free.cpp)
  — show paths that runtime sanitizers can miss. Need GCC 16 for usable C++
  analyzer support; CI runs them via `cmake -DGCC_FEATURE_ANALYZER=ON`.

Full indexes: [features/gccext/attributes/README.md](features/gccext/attributes/README.md), [builtins/](features/gccext/builtins/README.md), [codegen/](features/gccext/codegen/README.md), [openmp/](features/gccext/openmp/README.md), [pragmas/](features/gccext/pragmas/README.md), [sanitize/](features/gccext/sanitize/README.md), [analyzer/](features/gccext/analyzer/README.md).

### 6. Edge — C++26 frontier + per-release smoke tests

Every C++26 entry has an explicit proof mode and a version gate. Supported
features run normally; negative examples must fail with a matched diagnostic;
the one header-only snapshot is marked `COMPILE_ONLY` with its limitation in
`coverage.yml`. Unsupported facilities remain visible as known gaps.

- **Language:** [`cpp26_pack_indexing`](features/std/cpp26/cpp26_pack_indexing.cpp),
  [`cpp26_delete_reason`](features/std/cpp26/cpp26_delete_reason.cpp),
  [`cpp26_expansion_statements`](features/std/cpp26/cpp26_expansion_statements.cpp),
  [`cpp26_structured_binding_pack`](features/std/cpp26/cpp26_structured_binding_pack.cpp),
  [`cpp26_constexpr_exceptions`](features/std/cpp26/cpp26_constexpr_exceptions.cpp),
  [`cpp26_static_assert_messages`](features/std/cpp26/cpp26_static_assert_messages.cpp),
  [`cpp26_contracts_basic`](features/std/cpp26/cpp26_contracts_basic.cpp).
- **Library:** [`cpp26_saturation_arith`](features/std/cpp26/cpp26_saturation_arith.cpp),
  [`cpp26_inplace_vector`](features/std/cpp26/cpp26_inplace_vector.cpp),
  [`cpp26_optional_ref`](features/std/cpp26/cpp26_optional_ref.cpp),
  [`cpp26_function_wrappers`](features/std/cpp26/cpp26_function_wrappers.cpp),
  [`cpp26_indirect_polymorphic`](features/std/cpp26/cpp26_indirect_polymorphic.cpp),
  [`cpp26_simd`](features/std/cpp26/cpp26_simd.cpp),
  [`cpp26_submdspan`](features/std/cpp26/cpp26_submdspan.cpp),
  [`cpp26_philox_engine`](features/std/cpp26/cpp26_philox_engine.cpp),
  [`cpp26_span_at`](features/std/cpp26/cpp26_span_at.cpp),
  [`cpp26_text_encoding`](features/std/cpp26/cpp26_text_encoding.cpp).
- **Reflection:** [`cpp26_reflection_basic`](features/std/cpp26/cpp26_reflection_basic.cpp)
  (GCC 16, `-freflection`).

Per-release smoke tests: [`gcc13_libstdcxx_format`](features/gcc/gcc13/gcc13_libstdcxx_format.cpp),
[`gcc14_libstdcxx_ranges_to`](features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp)
+ [`gcc14_libstdcxx_print_exp`](features/gcc/gcc14/gcc14_libstdcxx_print_exp.cpp),
[`gcc15_default_print`](features/gcc/gcc15/gcc15_default_print.cpp),
[`gcc16_cpp26_features_default`](features/gcc/gcc16/gcc16_cpp26_features_default.cpp).
The narrative version of "what each release shipped" is in
[docs/gcc-changelogs.md](docs/gcc-changelogs.md).

Toolchain *defaults* get their own bucket:
[`gccdef_dialect`](features/gcc/defaults/gccdef_dialect.cpp) (what you get
with no `-std` — changed at GCC 15 for C and GCC 16 for C++),
[`gccdef_fp_contract`](features/gcc/defaults/gccdef_fp_contract.cpp)
(`-ffp-contract=fast` is always on; AArch64 twin
[`gccdef_fp_contract_aarch64`](features/gcc/defaults/gccdef_fp_contract_aarch64.cpp)), and
[`gccdef_pie_default`](features/gcc/defaults/gccdef_pie_default.cpp)
(upstream vs distro packaging). Each release bucket also carries one demo of
a diagnostic that release introduced (`ctest -L gcc-diagnostics`):
[`gcc13_warn_dangling_reference`](features/gcc/gcc13/gcc13_warn_dangling_reference.cpp),
[`gcc14_warn_calloc_transposed`](features/gcc/gcc14/gcc14_warn_calloc_transposed.cpp),
[`gcc15_warn_deprecated_literal_operator`](features/gcc/gcc15/gcc15_warn_deprecated_literal_operator.cpp),
[`gcc16_warn_cpp26_compat`](features/gcc/gcc16/gcc16_warn_cpp26_compat.cpp)
— each compiled with `-Werror=<warning>` and asserting the exact diagnostic.
Measured details in [docs/default-changes.md](docs/default-changes.md).

Full indexes: [features/std/cpp26/README.md](features/std/cpp26/README.md), [features/gcc/gcc13/README.md](features/gcc/gcc13/README.md), [features/gcc/gcc14/README.md](features/gcc/gcc14/README.md), [features/gcc/gcc15/README.md](features/gcc/gcc15/README.md), [features/gcc/gcc16/README.md](features/gcc/gcc16/README.md).

## Why this layout

- **Two-level subfolders under `features/` + per-folder `CMakeLists.txt`**
  keeps the tree navigable as it grows. Filename prefix (`cpp23_…`) is the
  bucket key; the matching `gcc_feature_test()` call in the folder's
  `CMakeLists.txt` carries the build metadata.
- **One toolchain source (official `gcc:N` Docker image)** for both local
  podman and CI means there is exactly one place a version pin can drift.
- **Explicit proof modes** distinguish runtime behavior, compile-time
  diagnostics, compiler reports, modules, and compile-only availability. A
  feature cannot pass by silently falling back to a no-op.
