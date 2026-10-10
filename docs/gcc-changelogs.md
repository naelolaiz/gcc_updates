# GCC release-notes highlights (13 → 16)

Curated cheat sheet of what each GCC release shipped, organised by area:
**front-end / language**, **libstdc++**, **optimization**, **diagnostics &
analyzer**, **sanitizers**, **target / codegen**. The release notes
themselves contain hundreds of bullet points — what's reproduced here is the
subset most often asked about, with cross-links to the matching examples in
[../features/](../features/). Every bullet is taken from the official
release notes linked under each section, or measured by an example here.

The repo also has a [gccext_*](../features/gccext/) bucket for *non-version-specific*
GCC features (extension attributes, builtins, OpenMP, target multi-versioning,
vector extensions, etc.) — those are mentioned by name below where relevant.

> Always read the upstream notes for the version you actually use:
> <https://gcc.gnu.org/releases.html>. The lists below are highlights, not
> a substitute.

---

## GCC 13 (April 2023)

### Front-end / language

- Several C++23 features under `-std=c++23` (still experimental), among
  them portable assumptions — `[[assume(expr)]]`, P1774R8 — see
  [features/std/cpp23/cpp23_assume.cpp](../features/std/cpp23/cpp23_assume.cpp).
- `static operator()` (P1169R4) and `static operator[]` (P2589R1) — see
  [features/std/cpp23/cpp23_static_operator.cpp](../features/std/cpp23/cpp23_static_operator.cpp).
- Many C23 features in C: `nullptr`, `auto`, `constexpr` objects,
  `typeof_unqual`, enhanced enumerations.

### libstdc++

- **`<format>`** ships with a usable implementation for the first time —
  see [features/std/cpp20/cpp20_format.cpp](../features/std/cpp20/cpp20_format.cpp) and
  [features/gcc/gcc13/gcc13_libstdcxx_format.cpp](../features/gcc/gcc13/gcc13_libstdcxx_format.cpp).
  `<chrono>` gains time zones, the extra C++20 clocks, and `std::format`
  support.
- C++23 views: `zip`, `zip_transform`, `adjacent`, `adjacent_transform`,
  `pairwise`, `slide`, `chunk`, `chunk_by`, `repeat`, `cartesian_product`,
  `as_rvalue`, `as_const`, `enumerate`.
- C++23 algorithms in `<algorithm>`: `ranges::contains`,
  `contains_subrange`, `iota`, `find_last*`, and the `fold_*` family — see
  [cpp23_ranges_fold.cpp](../features/std/cpp23/cpp23_ranges_fold.cpp).
- Monadic operations for `std::expected`, `constexpr` `std::bitset` and
  `<charconv>`, and the `<stdfloat>` extended floating-point types — see
  [cpp23_stdfloat.cpp](../features/std/cpp23/cpp23_stdfloat.cpp).

### Optimization

- `-march=znver4` (AMD Zen 4) lets the auto-vectorizer consider 512-bit
  vectors.
- Parallel LTO WPA streaming cooperates with an active `make` jobserver.

### Diagnostics & analyzer

- New C++ warnings `-Wdangling-reference` (see
  [gcc13_warn_dangling_reference.cpp](../features/gcc/gcc13/gcc13_warn_dangling_reference.cpp))
  and `-Wself-move`; new C warning `-Wenum-int-mismatch`.
- `-fanalyzer` gains about 20 new warnings, but the release notes still call
  it suitable only for C code.

### Sanitizers

- AddressSanitizer defaults to `detect_stack_use_after_return=1` on
  GNU/Linux targets.

### Target / codegen

- AMD Zen 4 (`-march=znver4`) support and tuning.
- LoongArch gains `-mdirect-extern-access` and support for more sanitizers.

Source: <https://gcc.gnu.org/gcc-13/changes.html>

---

## GCC 14 (May 2024)

### Front-end / language

- First C++26 features, for example user-generated `static_assert`
  messages (P2741R3, see
  [cpp26_static_assert_messages.cpp](../features/std/cpp26/cpp26_static_assert_messages.cpp)),
  the `_` placeholder variable (P2169R4), `constexpr` cast from `void*`
  (P2738R1), and trivial infinite loops no longer being UB (P2809R3).
- C++23: deducing `this` (P0847R7, see
  [cpp23_deducing_this.cpp](../features/std/cpp23/cpp23_deducing_this.cpp)),
  `consteval` propagation (P2564R3), and unknown references in constant
  expressions (P2280R4).
- C: `-std=c23` / `-std=gnu23` are accepted, and bit-precise integers
  (`_BitInt(N)`) arrive on x86-64 and AArch64.

### libstdc++

- **`std::ranges::to`** is added — see
  [features/std/cpp23/cpp23_ranges_to.cpp](../features/std/cpp23/cpp23_ranges_to.cpp) and
  [features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp](../features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp).
- **`std::generator`** (C++23) — coroutine-backed lazy generator. See
  [features/std/cpp23/cpp23_generator.cpp](../features/std/cpp23/cpp23_generator.cpp).
- **`std::print` / `std::println`** (C++23) — formatted output straight to a
  `FILE*`. On Linux they link with default flags; only Windows builds need
  `-lstdc++exp` for Unicode console output. See
  [features/std/cpp23/cpp23_print.cpp](../features/std/cpp23/cpp23_print.cpp) and
  [features/gcc/gcc14/gcc14_libstdcxx_print.cpp](../features/gcc/gcc14/gcc14_libstdcxx_print.cpp).
- **`std::stacktrace`** (C++23) — `<stacktrace>` is enabled by default, but
  its symbols live in `libstdc++exp.a`, so link with `-lstdc++exp` (still
  true in GCC 16). See
  [features/std/cpp23/cpp23_stacktrace.cpp](../features/std/cpp23/cpp23_stacktrace.cpp).
- `std::out_ptr` / `std::inout_ptr`, and formatters for `std::thread::id`
  and `std::stacktrace`.
- C++26: saturation arithmetic (`std::add_sat` and friends, see
  [cpp26_saturation_arith.cpp](../features/std/cpp26/cpp26_saturation_arith.cpp)),
  `std::text_encoding` (see
  [cpp26_text_encoding.cpp](../features/std/cpp26/cpp26_text_encoding.cpp)),
  `std::runtime_format`, and `std::to_string` defined in terms of
  `std::format`.

### Optimization & hardening

- New `-fhardened` umbrella option that turns on a curated set of hardening
  flags (list them with `--help=hardened`) — see
  [gcc14_hardened_bundle.cpp](../features/gcc/gcc14/gcc14_hardened_bundle.cpp).
- The vectorizer handles loops that contain any number of early breaks.

### Diagnostics & analyzer

- New `-Wnrvo` warns when named return value optimization is allowed but not
  performed.
- `-Wcalloc-transposed-args` and `-Walloc-size` now also apply to C++ — see
  [gcc14_warn_calloc_transposed.cpp](../features/gcc/gcc14/gcc14_warn_calloc_transposed.cpp).
- `-fanalyzer` adds new warnings and enables taint tracking by default; the
  release notes still describe it as suitable only for C.

### Sanitizers

- Hardware-assisted AddressSanitizer works on x86-64 with LAM_U57.

### Target / codegen

- AArch64: SME and SME2 support (`+sme`, `+sme2`).
- x86: Intel APX and AVX10.1 support.
- OpenMP offloading: low-latency allocators on AMD GCN and nvptx devices.

Source: <https://gcc.gnu.org/gcc-14/changes.html>

---

## GCC 15 (April 2025)

### Front-end / language

- Default C standard bumped to **C23** (`gnu23`).
- COBOL front-end added.
- C++26 additions include pack indexing (P2662R3) and deleted functions with
  a reason (P2573R2) — see
  [cpp26_pack_indexing.cpp](../features/std/cpp26/cpp26_pack_indexing.cpp)
  and [cpp26_delete_reason.cpp](../features/std/cpp26/cpp26_delete_reason.cpp) —
  plus `#embed` (P1967R14, see
  [cpp26_embed.cpp](../features/std/cpp26/cpp26_embed.cpp)), attributes on
  structured bindings (P0609R3), and variadic friends (P2893R3).
- `constexpr`-generated strings are accepted in inline assembler statements.
- C++ modules are "greatly improved"; this repository's interface → importer
  → link → run fixture,
  [cpp20_modules_basic.cpp](../features/std/cpp20/cpp20_modules_basic.cpp),
  passes from GCC 15 on (still behind `-fmodules`).

### libstdc++

- **`std::flat_map` / `std::flat_set`** (C++23) ship for the first time — see
  [cpp23_flat_map.cpp](../features/std/cpp23/cpp23_flat_map.cpp) and
  [cpp23_flat_set.cpp](../features/std/cpp23/cpp23_flat_set.cpp).
- **Formatting ranges and tuples** (C++23): `std::format("{}", vec)` and
  `std::println("{}", vec)` work — see
  [cpp23_format_ranges.cpp](../features/std/cpp23/cpp23_format_ranges.cpp) and
  [gcc15_libstdcxx_print_ranges.cpp](../features/gcc/gcc15/gcc15_libstdcxx_print_ranges.cpp).
- The `std` and `std.compat` modules (`import std;`), also usable from C++20.
- `from_range_t` constructors and `insert_range` / `append_range` for
  containers (C++23) — see
  [cpp23_ranges_from_range.cpp](../features/std/cpp23/cpp23_ranges_from_range.cpp).
- C++26: `views::concat` (see
  [cpp26_views_concat.cpp](../features/std/cpp26/cpp26_views_concat.cpp)),
  `views::to_input`, `views::cache_latest`,
  `constexpr` sorting, `std::is_virtual_base_of`, and member `visit` for
  `std::variant`.
- Debug assertions are enabled by default in unoptimized builds; define
  `_GLIBCXX_NO_ASSERTIONS` to turn them off.

### Optimization

- Incremental LTO reduces recompilation time after small edits.

### Diagnostics & analyzer

- New `-Wdeprecated-literal-operator` for the C++23-deprecated
  `operator "" _x` spelling — see
  [gcc15_warn_deprecated_literal_operator.cpp](../features/gcc/gcc15/gcc15_warn_deprecated_literal_operator.cpp).
- Diagnostics that contrast two source locations use color, and SARIF output
  is richer.
- `-fanalyzer` improves path printing and adds
  `-Wanalyzer-undefined-behavior-ptrdiff`; the release notes still say C++
  output is unlikely to be meaningful.

### Target / codegen

- x86: AVX10.2 (`-mavx10.2`) and `-march=diamondrapids`; Knights Landing and
  Knights Mill support is removed.
- AArch64: SME2.1 and other new architecture extensions.

Source: <https://gcc.gnu.org/gcc-15/changes.html>

---

## GCC 16 (April 2026)

### Front-end / language

- The default C++ dialect changes from `gnu++17` to **`gnu++20`**. The
  [default dialect smoke test](../features/gcc/defaults/gccdef_dialect.cpp)
  measures this without passing `-std=`.
- C++20 modules remain experimental behind `-fmodules`. The new
  `--compile-std-module` option builds the `<bits/stdc++.h>` header unit and
  the `std` / `std.compat` modules in one step.
- C++26 reflection (P2996R13) ships behind `-freflection`; expansion
  statements, structured-binding packs, constexpr exceptions, contracts, and
  erroneous reads of uninitialized values are also covered by executable or
  diagnostic tests under [features/std/cpp26/](../features/std/cpp26/).
- Algol 68 front-end added.

### libstdc++

- The C++20 implementation is no longer experimental. Several C++20
  components (atomic waiting, semaphores, `<syncstream>`, `std::format`, some
  `<ranges>` adaptors) changed ABI, so C++20 objects built with older
  releases are not compatible with GCC 16 ones.
- Strict `-std=c++NN` modes now classify `__int128` as integral; see
  [gcc16_int128_type_traits.cpp](../features/gcc/gcc16/gcc16_int128_type_traits.cpp).
- Newly testable facilities include `std::mdspan`, `std::ranges::shift_left`,
  `allocate_at_least`, `inplace_vector`, `optional<T&>`, `copyable_function`,
  `function_ref`, `indirect`, `polymorphic`, `submdspan`, `philox_engine`, and
  the C++26 `simd` API. Their individual GCC and libstdc++ gates are recorded
  in [coverage.yml](../coverage.yml). Also new: `std::constant_wrapper`,
  `std::owner_equal`, padded `mdspan` layouts, and `std::aligned_accessor`.
- `<debugging>` (`std::is_debugger_present`, `std::breakpoint_if_debugging`)
  is exported from `libstdc++exp.a`, like `std::stacktrace`; see
  [cpp26_debugging.cpp](../features/std/cpp26/cpp26_debugging.cpp), which
  links `-lstdc++exp`.

### Optimization

- The vectorizer generates more efficient code for loops with early breaks;
  with AVX-512 enabled it tries a masked vector epilogue.

### Target / codegen

- x86: `-march=novalake` (APX, AVX10.1, AVX10.2), `-march=znver6`, and
  `-march=wildcatlake`; `-mavx10.1-256`, `-mavx10.1-512`, and `-mevex512` are
  removed.

### Diagnostics & analyzer

- `-fanalyzer` handles C++ named return value optimization and starts to
  model exceptions (new `-fanalyzer-assume-nothrow`); CI runs the
  [analyzer demos](../features/gccext/analyzer/) on GCC 16.
- New `-Wc++26-compat` flags identifiers that become C++26 keywords — see
  [gcc16_warn_cpp26_compat.cpp](../features/gcc/gcc16/gcc16_warn_cpp26_compat.cpp).

Source: <https://gcc.gnu.org/gcc-16/changes.html>

---

## How this maps to the examples

| Release smoke-test                                                                  |
|--------------------------------------------------------------------------------------|
| [features/gcc/gcc13/gcc13_libstdcxx_format.cpp](../features/gcc/gcc13/gcc13_libstdcxx_format.cpp)        |
| [features/gcc/gcc14/gcc14_libstdcxx_print.cpp](../features/gcc/gcc14/gcc14_libstdcxx_print.cpp)          |
| [features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp](../features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp)  |
| [features/gcc/gcc14/gcc14_hardened_bundle.cpp](../features/gcc/gcc14/gcc14_hardened_bundle.cpp)          |
| [features/gcc/gcc15/gcc15_libstdcxx_print_ranges.cpp](../features/gcc/gcc15/gcc15_libstdcxx_print_ranges.cpp) |
| [features/gcc/gcc16/gcc16_cpp26_features_default.cpp](../features/gcc/gcc16/gcc16_cpp26_features_default.cpp) |
| [features/gcc/gcc16/gcc16_int128_type_traits.cpp](../features/gcc/gcc16/gcc16_int128_type_traits.cpp)    |

For non-version-specific GCC features that are *always* there (attributes,
builtins, OpenMP, vector_size types, target multi-versioning, etc.), browse
the [gccext index](../features/gccext/README.md). The per-topic indexes
([attributes/](../features/gccext/attributes/README.md),
[builtins/](../features/gccext/builtins/README.md),
[codegen/](../features/gccext/codegen/README.md),
[openmp/](../features/gccext/openmp/README.md),
[pragmas/](../features/gccext/pragmas/README.md),
[sanitize/](../features/gccext/sanitize/README.md),
[analyzer/](../features/gccext/analyzer/README.md))
list the examples whose build metadata is declared in each folder's
`CMakeLists.txt`.

## How non-runtime claims are proved

A few classes of compiler features can't be exercised through this repo's
"compile + run + assert output" harness:

- **Auto-vectorization.** The autovectorization example separately proves
  numerical correctness and matches GCC's `-fopt-info-vec-optimized` report
  for `loop vectorized`. It is skipped under sanitizers because instrumentation
  deliberately changes code-generation decisions.
- **LTO / PGO** improve speed without changing observable behaviour. Add
  `-flto` to a build command to test it; correctness assertions still hold.
- **`-fanalyzer` (the static analyzer)** emits warnings, not binaries. The
  analyzer lane compiles each deliberate defect at `-O0 -fanalyzer` and fails
  unless the expected `-Wanalyzer-*` category appears. Run it with
  `./scripts/container-dev.sh 16 analyzer`.
- **Sanitizers** (`-fsanitize=address,undefined,thread`) abort at runtime
  on errors but don't change the *successful* path. The deliberate-trip demos
  under [../features/gccext/sanitize/](../features/gccext/sanitize/) get around
  this with `WILL_FAIL` plus `EXPECT_OUTPUT`, so we assert the expected report
  text instead of accepting any failure. CI runs separate sanitizer jobs for
  UBSan+ASan+LSan and TSan, plus the `analyze` job above.
