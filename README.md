# gcc_updates

A structured, CI-tested workspace serving as a reference for the C++11 / C++17 /
C++20 / C++23 / (experimental) C++26 standard library and tracking what each new
GCC release ships.

Every example under [features/](features/) is a single-file program with a
metadata header. A small Python engine ([scripts/discover.py](scripts/discover.py))
discovers each example, builds it with the right `-std=` flag against the
installed `g++`, runs it, and checks the exit code. CI runs that engine four
times — once each on GCC 13, 14, 15 and 16 — so any breakage shows up per-row.

## Layout

```
features/                          # all examples (single .cpp each)
  std/                             # C++ standard library + language features
    cpp11/cpp11_*.cpp              # foundation
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
  gcc/                             # release-notes smoke tests, one per version
    gcc13/gcc13_*.cpp
    gcc14/gcc14_*.cpp
    gcc15/gcc15_*.cpp
    gcc16/gcc16_*.cpp
scripts/
  discover.py                 # build & run engine
  run-tests.sh                # thin wrapper
  podman-dev.sh               # local entrypoint (uses podman)
containers/
  gcc.Containerfile           # ubuntu:24.04 + gcc-N from toolchain PPA
docs/
  cpp11.md / cpp17.md / cpp20.md / cpp23.md / cpp26.md   # auto-generated indexes
  gccext.md                          # auto-generated index of GCC extensions
  compiler-flags.md                  # default and per-file flag reference
  gcc-changelogs.md                # curated per-release notes
.github/workflows/
  ci.yml                      # matrix on gcc 13, 14, 15, 16 + sanitizer row
```

## How each file gets compiled

Every example is built with this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -O2 -pthread $EXTRA_FLAGS file.cpp -o /tmp/.../bin
```

`$STD` is read from the file's `// gcc-test:` header. `$EXTRA_FLAGS` is the
file's `extra-flags=` value if present (e.g. `-lstdc++exp` for `std::stacktrace`,
`-ltbb` for parallel algorithms). The full per-file command for any example
can be dumped with `--show-cmds`:

```bash
./scripts/podman-dev.sh 15 --show-cmds | head -3
```

[docs/compiler-flags.md](docs/compiler-flags.md) is the complete reference —
every default flag, every per-file `extra-flags=` value, and how to reproduce
any build by hand inside a container.

## Anatomy of one example

Each `.cpp` starts with a metadata header that the engine parses:

```cpp
// gcc-test: std=c++23 min-gcc=14 topic=ranges experimental=false
// description: std::ranges::to converts a range to a container in one expression.
// since: GCC 14 / libstdc++ 14
// reference: https://en.cppreference.com/w/cpp/ranges/to

#include <ranges>
#include <vector>
#include <list>
#include <cassert>

int main() {
    std::list<int> l{1, 2, 3, 4, 5};
    auto v = l | std::ranges::to<std::vector<int>>();
    assert(v.size() == 5);
    return 0;
}
```

Required keys: `std`, `min-gcc`, `topic`, `experimental`.
Optional keys: `extra-flags=-foo,-bar`, `run-args="..."`, `expect-exit=N`,
`max-gcc=N`. Programs return `0` on success; `assert(...)` is the standard way
to "test" the feature.

## Running locally (podman)

This repo never invokes the host's `g++` directly. Local runs go through
[scripts/podman-dev.sh](scripts/podman-dev.sh), which builds a small image from
[containers/gcc.Containerfile](containers/gcc.Containerfile) (ubuntu:24.04 + the
ubuntu-toolchain-r/test PPA) and runs the engine inside it:

```bash
./scripts/podman-dev.sh 13            # build + run all GCC-13-eligible examples
./scripts/podman-dev.sh 14            # same, GCC 14
./scripts/podman-dev.sh 15            # same, GCC 15
./scripts/podman-dev.sh 16            # same, GCC 16
./scripts/podman-dev.sh 15 --dry-run  # parse metadata only, don't compile
./scripts/podman-dev.sh 15 --filter=threading   # only threading/sync examples
```

The image is built once per GCC version and cached.

## Running in CI

[.github/workflows/ci.yml](.github/workflows/ci.yml) runs four matrix rows
(`gcc: [13, 14, 15, 16]`) on `ubuntu-latest`. Each row installs that GCC version
from the toolchain PPA and runs the engine. Per-example PASS/SKIP lines fold
into GitHub `::group::` sections so the run page surfaces FAIL lines and the
summary without scrolling.

A second job (`sanitize`) re-runs every example under `-fsanitize=undefined,address`
on GCC 15 — see [docs/sanitizers.md](docs/sanitizers.md).

### Optional: install the pre-commit hook

The auto-generated per-leaf `features/<bucket>/README.md` indexes are kept fresh
by an opt-in pre-commit hook in [.githooks/pre-commit](.githooks/pre-commit).
On commits that touch `features/`, the hook runs `discover.py --emit-docs`
(pure stdlib Python — no compiler, no container needed) and stages any updated
README.md files into the same commit. Enable it once with:

```bash
git config core.hooksPath .githooks
```

If you skip the hook, the indexes simply lag the metadata until the next
contributor's commit refreshes them — CI no longer gates on this.

## Adding a new example

1. Drop a single `.cpp` under [features/](features/) in the right subfolder:
   `features/std/cppNN/`, `features/gcc/gccNN/`, or
   `features/gccext/<topic>/`. Filename must start with the bucket prefix
   (`cpp11_`, `cpp20_`, `gcc14_`, `gccext_`, …) — the engine reads the bucket
   from the filename.
2. Add the `// gcc-test:` metadata header (required keys above) and a
   `// description:` line.
3. Write `int main()` that asserts what should hold, returns 0 on success.
4. Run `./scripts/podman-dev.sh <ver>` to verify locally.
5. Commit. If you've enabled the pre-commit hook (above), the per-leaf
   `features/<bucket>/README.md` index is regenerated and staged automatically.
   Otherwise run `./scripts/podman-dev.sh <ver> --emit-docs` and commit the
   regenerated index alongside the example.

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
- **Templates:** `cpp11_variadic_templates`, `cpp11_alias_template`.
- **STL value types:** `cpp11_array`, `cpp11_tuple`, `cpp11_unordered_map`,
  `cpp11_initializer_list`, `cpp11_smart_ptrs`, `cpp11_chrono`, `cpp11_random`,
  `cpp11_regex`.
- **Callables:** `cpp11_function`, `cpp11_bind`.
- **Concurrency:** `cpp11_thread`, `cpp11_mutex_lock`,
  `cpp11_condition_variable`, `cpp11_async_future`, `cpp11_atomic`.
- **Niceties:** `cpp11_raw_string`, `cpp11_user_defined_literal`,
  `cpp11_alignof_alignas`, `cpp11_attribute_noreturn`,
  `cpp11_inline_namespace`.

Full index: [features/std/cpp11/README.md](features/std/cpp11/README.md).

### 2. Quality-of-life (C++17)

- **STL value types:** `cpp17_optional`, `cpp17_variant`, `cpp17_any`,
  `cpp17_string_view`, `cpp17_byte`, `cpp17_filesystem`.
- **Algorithms:** `cpp17_clamp`, `cpp17_invoke_apply`, `cpp17_parallel_algos`.
- **Templates:** `cpp17_if_constexpr`, `cpp17_fold_expressions`,
  `cpp17_ctad_basic`.
- **Language:** `cpp17_structured_bindings`, `cpp17_init_statement`,
  `cpp17_inline_variables`, `cpp17_nested_namespace`,
  `cpp17_constexpr_lambda`, `cpp17_attributes`.
- **Concurrency:** `cpp17_shared_mutex`.

Full index: [features/std/cpp17/README.md](features/std/cpp17/README.md).

### 3. The big jump (C++20)

This is where the language genuinely changed shape.

- **Concepts & templates:** `cpp20_concepts_intro`,
  `cpp20_concepts_requires_expr`, `cpp20_ctad_alias`,
  `cpp20_ctad_aggregates`, `cpp20_nttp_class`, `cpp20_explicit_bool`.
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
  `cpp23_out_ptr`, `cpp23_string_contains`, `cpp23_resize_and_overwrite`.

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

Full indexes: [features/gccext/attributes/README.md](features/gccext/attributes/README.md), [builtins/](features/gccext/builtins/README.md), [codegen/](features/gccext/codegen/README.md), [openmp/](features/gccext/openmp/README.md), [pragmas/](features/gccext/pragmas/README.md), [sanitize/](features/gccext/sanitize/README.md).

### 6. Edge — experimental C++26 + per-release smoke tests

C++26 examples are flagged `experimental=true`; CI tolerates failures here.

- `cpp26_saturation_arith`, `cpp26_contracts_basic`, `cpp26_reflection_basic`.

Per-release smoke tests: `gcc13_libstdcxx_format`, `gcc14_libstdcxx_ranges_to`,
`gcc15_default_print`, `gcc16_cpp26_features_default`. The narrative version of
"what each release shipped" is in
[docs/gcc-changelogs.md](docs/gcc-changelogs.md).

Full indexes: [features/std/cpp26/README.md](features/std/cpp26/README.md), [features/gcc/gcc13/README.md](features/gcc/gcc13/README.md), [features/gcc/gcc14/README.md](features/gcc/gcc14/README.md), [features/gcc/gcc15/README.md](features/gcc/gcc15/README.md), [features/gcc/gcc16/README.md](features/gcc/gcc16/README.md).

## Why this layout

- **Two-level subfolders under `features/` + metadata header** keeps the tree
  navigable as it grows. Filename prefix (`cpp23_…`) doubles as the bucket
  key so the engine and emit-docs both work off the same fact.
- **One toolchain source (toolchain-r PPA)** for both local podman and CI means
  there is exactly one place a version pin can drift.
- **Experimental flag** lets C++26 / cutting-edge GCC features live in the same
  matrix without making every PR red whenever a feature breaks.
