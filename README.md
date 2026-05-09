# gcc_updates

A structured, CI-tested workspace serving as a reference for the C++11 / C++17 /
C++20 / C++23 / (experimental) C++26 standard library and tracking what each new
GCC release ships.

Every example under [features/](features/) is a single-file program registered
as one `gcc_feature_test()` call in its folder's `CMakeLists.txt`. CMake +
CTest drive the build and run; the function macro lives in
[cmake/GccFeature.cmake](cmake/GccFeature.cmake). CI runs the matrix three
times on GCC 13/14/15 plus three more on GCC 15/16 under UBSan+ASan, TSan,
and `-fanalyzer` — so correctness, runtime UB, data races, and compile-time
path analysis are all covered per push. (The `gcc:16` Docker image isn't
published yet; the matrix re-adds it once it lands. The analyzer job already
exercises GCC 16 via `debian:unstable-slim` + apt.)

## Layout

```
features/                          # all examples (single .cpp each)
  support/demo.hpp                 # tiny stdout helper + DEMO_ASSERT for readable demos
  std/                             # C++ standard library + language features
    cpp11/cpp11_*.cpp              # foundation
    cpp14/cpp14_*.cpp
    cpp17/cpp17_*.cpp
    cpp20/cpp20_*.cpp
    cpp23/cpp23_*.cpp
    cpp26/cpp26_*.cpp              # experimental C++26 (failures don't break CI)
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
    analyzer/gccext_analyzer_*.cpp # compile-time -fanalyzer demos (UAF/null-deref/double-delete)
  gcc/                             # release-notes smoke tests, one per version
    gcc13/gcc13_*.cpp
    gcc14/gcc14_*.cpp
    gcc15/gcc15_*.cpp
    gcc16/gcc16_*.cpp
CMakeLists.txt                 # top-level: discovers all features/<bucket>/ subdirs
cmake/
  GccFeature.cmake             # gcc_feature_test() function and registration validation
  expect_failure.cmake         # command runner for expected failures with diagnostic regexes
features/<bucket>/CMakeLists.txt  # one gcc_feature_test() call per .cpp in the bucket
features/<bucket>/README.md     # per-bucket index (manual, hand-edited)
scripts/
  podman-dev.sh               # local entrypoint (uses podman + cmake + ctest)
containers/
  gcc.Containerfile           # FROM gcc:${GCC_VERSION} + cmake + libtbb-dev
docs/
  compiler-flags.md           # default and per-file flag reference
  sanitizers.md               # what -DGCC_FEATURE_SANITIZE=… adds, runtime knobs
  gcc-changelogs.md           # curated per-release notes (GCC 13 → 16)
.github/workflows/
  ci.yml                      # gcc 13/14/15 matrix + ubsan+asan + tsan + analyzer jobs
```

## How each file gets compiled

Every example is built with this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -O2 -pthread -Ifeatures file.cpp $EXTRA -o build/<bucket>/bin
```

`$STD` is the `STD` argument of the example's `gcc_feature_test()` call.
`$EXTRA` is its `EXTRA_LIBS` / `EXTRA_COMPILE_FLAGS` (e.g. `stdc++exp` for
`std::stacktrace`, `tbb` for parallel algorithms). Sanitizer mode swaps
`-O2` for `-O1` and adds `-fsanitize=…`; analyzer mode adds `-fanalyzer`.

[docs/compiler-flags.md](docs/compiler-flags.md) is the complete reference —
every default flag, every per-test `EXTRA_*`, and how to reproduce any
build by hand inside a container.

## Anatomy of one example

Each `.cpp` is plain code with a `description` and `reference` comment block;
the build metadata lives in the folder's `CMakeLists.txt`:

```cpp
// description: std::ranges::to converts a range to a container in one expression.
// reference: https://en.cppreference.com/w/cpp/ranges/to

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
`EXPECT_OUTPUT`, `EXPERIMENTAL`, `EXPECT_ERROR`. See
[cmake/GccFeature.cmake](cmake/GccFeature.cmake) for the full grammar.
Programs return `0` on success; runtime checks use `DEMO_ASSERT(...)`.

## Running locally (podman)

This repo never invokes the host's `g++`/`cmake` directly. Local runs go
through [scripts/podman-dev.sh](scripts/podman-dev.sh), which builds a small
image from [containers/gcc.Containerfile](containers/gcc.Containerfile) (the
official `gcc:N` Docker image + `cmake` + `libtbb-dev`), then runs cmake +
ctest inside it. The build directory lives in a named podman volume, so it
persists across runs.

```bash
./scripts/podman-dev.sh 13            # build + run all GCC-13-eligible tests
./scripts/podman-dev.sh 14            # same, GCC 14
./scripts/podman-dev.sh 15            # same, GCC 15
./scripts/podman-dev.sh 16            # same, GCC 16

# CTest filters (everything after `--` is forwarded as-is to ctest):
./scripts/podman-dev.sh 15 -- -R cpp23_         # only tests matching cpp23_
./scripts/podman-dev.sh 15 -- -L threading      # only the 'threading' topic
./scripts/podman-dev.sh 15 -- -j                # parallel CTest when speed matters
./scripts/podman-dev.sh 15 -- -N                # list tests, don't run

# Sanitizer / analyzer modes:
./scripts/podman-dev.sh 15 sanitize=address,undefined
./scripts/podman-dev.sh 15 sanitize=thread
./scripts/podman-dev.sh 16 analyzer
```

The image is built once per GCC version and cached.

## Running in CI

[.github/workflows/ci.yml](.github/workflows/ci.yml) defines four jobs. Each
default and sanitizer job runs inside the official `gcc:N` Docker image
(Debian-based, upstream gcc-N + matching upstream libstdc++-N). The same image
is what `scripts/podman-dev.sh` builds locally, so behaviour is bit-identical
between local and CI for those jobs. The analyzer job uses
`debian:unstable-slim` until Docker Hub publishes `gcc:16`.

| Job | What it runs | Picks up |
|-----|--------------|----------|
| `gcc-{13,14,15}` | `cmake -S . -B build && ctest --test-dir build --verbose` (one row per version, in `gcc:N`) | every test whose `MIN_GCC` ≤ N and `MAX_GCC` ≥ N |
| `sanitize (gcc-15, ubsan + asan + lsan)` | `cmake -DGCC_FEATURE_SANITIZE=undefined,address` in `gcc:15` | every test **plus** `REQUIRES_SANITIZER` demos for {undefined, address, leak} |
| `sanitize (gcc-15, tsan)` | `cmake -DGCC_FEATURE_SANITIZE=thread` in `gcc:15` (separate; can't share with ASan) | every test plus `REQUIRES_SANITIZER thread` demos |
| `analyze (gcc-16, -fanalyzer)` | `cmake -DGCC_FEATURE_ANALYZER=ON` in `debian:unstable-slim` with `g++-16` installed from apt | `REQUIRES_ANALYZER` compile-only demos |

Each CTest run uses `--verbose` so the example output remains visible in the
log. Tests gated out by version / sanitizer / analyzer requirements are not
registered in that configure mode.

See [docs/sanitizers.md](docs/sanitizers.md) for what each sanitizer adds to
the build and how the deliberate-trip demos assert their expected report text.

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
| `std::ranges::starts_with` / `ends_with` | 16 |

These are also in `<algorithm>` (not `<ranges>`); forgetting that
include is the most common reason a `ranges::xxx` algorithm appears
"missing".

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

1. Drop a single `.cpp` under [features/](features/) in the right subfolder:
   `features/std/cppNN/`, `features/gcc/gccNN/`, or
   `features/gccext/<topic>/`. Filename must start with the bucket prefix
   (`cpp11_`, `cpp20_`, `gcc14_`, `gccext_`, …).
2. Add a `// description:` and `// reference:` comment block at the top of
   the `.cpp` file (no machine-readable header needed any more).
3. Write `int main()` that asserts what should hold, returns 0 on success.
4. Add a `gcc_feature_test(<name> STD c++NN MIN_GCC N TOPIC <topic>)` line
   to that folder's `CMakeLists.txt`. See
   [cmake/GccFeature.cmake](cmake/GccFeature.cmake) for the full grammar.
5. Run `./scripts/podman-dev.sh <ver>` to verify locally.
6. Update the bucket's `features/<bucket>/README.md` index by hand if you
   want it to list your new example. (Auto-generation from CMake target
   properties may come later.)

## Suggested reference path

The examples are independent — each `.cpp` is self-contained — but if you're
walking through them as a structured tour, this is the order I'd suggest. Each
step links to the per-bucket index that lists every example for that standard,
grouped by topic.

### 1. Foundation (C++11)

If you only know pre-C++11, this is the unavoidable starting point. Everything
that came after assumes you have these in muscle memory.

- **Core language:** `cpp11_auto`, `cpp11_range_for`, `cpp11_nullptr`,
  `cpp11_scoped_enum`, `cpp11_uniform_init`, `cpp11_lambda`, `cpp11_constexpr`,
  `cpp11_noexcept`, `cpp11_static_assert`, `cpp11_override_final`,
  `cpp11_delete_default`, `cpp11_trailing_return`, `cpp11_explicit_conversion`.
- **Move semantics (the hardest one — read it twice):** `cpp11_rvalue_move`.
- **Templates / metaprogramming:** `cpp11_variadic_templates`,
  `cpp11_alias_template`, `cpp11_type_traits`, `cpp11_ratio`.
- **STL value types:** `cpp11_array`, `cpp11_tuple`, `cpp11_unordered_map`,
  `cpp11_initializer_list`, `cpp11_smart_ptrs`, `cpp11_chrono`, `cpp11_random`,
  `cpp11_regex`, `cpp11_forward_list`.
- **STL infrastructure:** `cpp11_emplace`, `cpp11_hash`,
  `cpp11_reference_wrapper`, `cpp11_iterator_helpers`,
  `cpp11_algorithms_new`, `cpp11_iota`, `cpp11_system_error`,
  `cpp11_exception_ptr`.
- **Callables:** `cpp11_function`, `cpp11_bind`.
- **Concurrency:** `cpp11_thread`, `cpp11_mutex_lock`,
  `cpp11_condition_variable`, `cpp11_async_future`, `cpp11_atomic`.
- **Niceties:** `cpp11_raw_string`, `cpp11_user_defined_literal`,
  `cpp11_alignof_alignas`, `cpp11_attribute_noreturn`,
  `cpp11_inline_namespace`.

Full index: [features/std/cpp11/README.md](features/std/cpp11/README.md).

### 1.5 Spot-fills (C++14)

- **Templates / metaprogramming:** `cpp14_integer_sequence`.

Full index: [features/std/cpp14/README.md](features/std/cpp14/README.md).

### 2. Quality-of-life (C++17)

- **STL value types:** `cpp17_optional`, `cpp17_variant`, `cpp17_any`,
  `cpp17_string_view`, `cpp17_byte`, `cpp17_filesystem`,
  `cpp17_shared_ptr_array`.
- **Algorithms / numerics:** `cpp17_clamp`, `cpp17_invoke_apply`,
  `cpp17_parallel_algos`, `cpp17_gcd_lcm_sample`, `cpp17_not_fn_as_const`.
- **STL infrastructure:** `cpp17_charconv`, `cpp17_memory_resource`,
  `cpp17_node_handles`, `cpp17_launder`.
- **Templates / metaprogramming:** `cpp17_if_constexpr`, `cpp17_fold_expressions`,
  `cpp17_ctad_basic`, `cpp17_type_trait_helpers`.
- **Language:** `cpp17_structured_bindings`, `cpp17_init_statement`,
  `cpp17_inline_variables`, `cpp17_nested_namespace`,
  `cpp17_constexpr_lambda`, `cpp17_attributes`.
- **Concurrency:** `cpp17_shared_mutex`, `cpp17_scoped_lock`.

Full index: [features/std/cpp17/README.md](features/std/cpp17/README.md).

### 3. The big jump (C++20)

This is where the language genuinely changed shape.

- **Concepts & templates / metaprogramming:** `cpp20_concepts_intro`,
  `cpp20_concepts_requires_expr`, `cpp20_ctad_alias`,
  `cpp20_ctad_aggregates`, `cpp20_nttp_class`, `cpp20_explicit_bool`,
  `cpp20_type_identity`.
- **Ranges & views:** `cpp20_ranges_algorithms`, `cpp20_ranges_views`,
  `cpp20_views_keys_values`.
- **Coroutines:** `cpp20_coroutine_generator` (read after concepts; this is the
  hardest single feature in C++20).
- **Concurrency (the *big* upgrade):** `cpp20_jthread`, `cpp20_stop_token`,
  `cpp20_counting_semaphore`, `cpp20_binary_semaphore`, `cpp20_latch`,
  `cpp20_barrier`, `cpp20_atomic_wait`, `cpp20_atomic_ref`,
  `cpp20_atomic_shared_ptr`, `cpp20_atomic_flag_test`, `cpp20_syncstream`.
- **Library:** `cpp20_format`, `cpp20_span`, `cpp20_bit_ops`,
  `cpp20_source_location`, `cpp20_numbers`, `cpp20_chrono_calendar`,
  `cpp20_erase_if`, `cpp20_lerp_midpoint`, `cpp20_ssize`,
  `cpp20_string_starts_ends`, `cpp20_map_contains`, `cpp20_to_address`,
  `cpp20_endian`.
- **Language:** `cpp20_consteval`, `cpp20_spaceship`,
  `cpp20_designated_init`, `cpp20_lambdas`.

Full index: [features/std/cpp20/README.md](features/std/cpp20/README.md).

### 4. Polishing (C++23)

Mostly about smoothing C++20's rough edges plus a few headline items.

- **The headline items:** `cpp23_expected`, `cpp23_mdspan`, `cpp23_print`,
  `cpp23_generator`, `cpp23_stacktrace`, `cpp23_flat_map`, `cpp23_flat_set`.
- **Templates / language:** `cpp23_deducing_this` (huge), `cpp23_recursive_lambda`,
  `cpp23_static_operator`, `cpp23_multidim_subscript`,
  `cpp23_if_consteval`, `cpp23_assume`, `cpp23_size_t_literal`,
  `cpp23_auto_decay_copy`.
- **Ranges/views:** `cpp23_ranges_to`, `cpp23_ranges_zip`,
  `cpp23_ranges_chunk_slide`, `cpp23_ranges_enumerate`,
  `cpp23_ranges_join_with`, `cpp23_ranges_cartesian_product`,
  `cpp23_ranges_adjacent`, `cpp23_ranges_fold`,
  `cpp23_ranges_starts_ends_contains`, `cpp23_ranges_find_last`,
  `cpp23_ranges_iota_algorithm`, `cpp23_views_repeat`,
  `cpp23_views_stride`, `cpp23_views_as_const_as_rvalue`.
- **Library:** `cpp23_optional_monadic`, `cpp23_byteswap`,
  `cpp23_to_underlying`, `cpp23_unreachable`, `cpp23_move_only_function`,
  `cpp23_out_ptr`, `cpp23_string_contains`, `cpp23_resize_and_overwrite`,
  `cpp23_spanstream`, `cpp23_format_ranges`, `cpp23_forward_like`,
  `cpp23_start_lifetime_as`.

Full index: [features/std/cpp23/README.md](features/std/cpp23/README.md).

### 5. GCC-specific extensions (orthogonal to standards)

These features aren't in any C++ standard but ship with every modern GCC.
Read these alongside the standard-library bucket — `[[likely]]` is C++20 *and*
implemented via the same machinery as `__builtin_expect`, etc.

- **Attributes:** `gccext_likely_unlikely`, `gccext_attribute_pure_const`,
  `gccext_attribute_hot_cold`, `gccext_attribute_flatten`,
  `gccext_attribute_packed`, `gccext_attribute_cleanup`,
  `gccext_attribute_target` (per-function ISA selection),
  `gccext_target_clones` (multi-version + IFUNC dispatch).
- **Builtins:** `gccext_builtin_expect`, `gccext_builtin_constant_p`,
  `gccext_builtin_assume_aligned`.
- **Vectorization / SIMD:** `gccext_vector_extensions` (`vector_size` types),
  `gccext_autovectorize` (auto-vectorisable SAXPY at `-O3`).
- **Parallelism:** `gccext_openmp_parallel_for`.
- **Tooling:** `gccext_diagnostic_pragma`, `gccext_inline_asm`.
- **Sanitizer interaction (clean):** `gccext_no_sanitize_attribute`,
  `gccext_glibcxx_debug`, `gccext_assume_under_sanitize` — how to opt-in,
  opt-out, and reason about runtime checks. Live under
  [features/gccext/sanitize/integration/](features/gccext/sanitize/integration/).
- **Sanitizer trips (deliberate UB):** `gccext_asan_*`, `gccext_ubsan_*`,
  `gccext_tsan_*`, `gccext_lsan_*` — each one is gated on a matching
  `REQUIRES_SANITIZER` and asserts expected sanitizer report text instead of
  accepting any failure. Run with
  `./scripts/podman-dev.sh 15 sanitize=undefined,address` (or
  `sanitize=thread`).
- **Static analyzer (compile-only):** `gccext_analyzer_double_free`,
  `gccext_analyzer_null_deref`, `gccext_analyzer_use_after_free` — show paths
  that runtime sanitizers can miss. Need GCC 16 for usable C++ analyzer
  support; CI runs them via `cmake -DGCC_FEATURE_ANALYZER=ON`.

Full indexes: [features/gccext/attributes/README.md](features/gccext/attributes/README.md), [builtins/](features/gccext/builtins/README.md), [codegen/](features/gccext/codegen/README.md), [openmp/](features/gccext/openmp/README.md), [pragmas/](features/gccext/pragmas/README.md), [sanitize/](features/gccext/sanitize/README.md), [analyzer/](features/gccext/analyzer/README.md).

### 6. Edge — experimental C++26 + per-release smoke tests

C++26 examples are flagged `experimental=true`; CI tolerates failures here.

- `cpp26_saturation_arith`, `cpp26_contracts_basic`, `cpp26_reflection_basic`.

Per-release smoke tests: `gcc13_libstdcxx_format`, `gcc14_libstdcxx_ranges_to`,
`gcc15_default_print`, `gcc16_cpp26_features_default`. The narrative version of
"what each release shipped" is in
[docs/gcc-changelogs.md](docs/gcc-changelogs.md).

Full indexes: [features/std/cpp26/README.md](features/std/cpp26/README.md), [features/gcc/gcc13/README.md](features/gcc/gcc13/README.md), [features/gcc/gcc14/README.md](features/gcc/gcc14/README.md), [features/gcc/gcc15/README.md](features/gcc/gcc15/README.md), [features/gcc/gcc16/README.md](features/gcc/gcc16/README.md).

## Why this layout

- **Two-level subfolders under `features/` + per-folder `CMakeLists.txt`**
  keeps the tree navigable as it grows. Filename prefix (`cpp23_…`) is the
  bucket key; the matching `gcc_feature_test()` call in the folder's
  `CMakeLists.txt` carries the build metadata.
- **One toolchain source (official `gcc:N` Docker image)** for both local
  podman and CI means there is exactly one place a version pin can drift.
- **`EXPERIMENTAL` keyword** lets C++26 / cutting-edge GCC features live in
  the same matrix without making every PR red whenever a feature breaks.
