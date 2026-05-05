# GCC release-notes highlights (13 → 16)

Curated cheat sheet of what each GCC release shipped, organised by area:
**front-end / language**, **libstdc++**, **optimization**, **diagnostics &
analyzer**, **sanitizers**, **target / codegen**, **tooling**. The release
notes themselves contain hundreds of bullet points — what's reproduced here is
the subset most often asked about, with cross-links to the matching examples
in [../features/](../features/).

The repo also has a [gccext_*](../features/) bucket for *non-version-specific*
GCC features (extension attributes, builtins, OpenMP, target multi-versioning,
vector extensions, etc.) — those are mentioned by name below where relevant.

> Always read the upstream notes for the version you actually use:
> <https://gcc.gnu.org/releases.html>. The lists below are highlights, not
> a substitute.

---

## GCC 13 (April 2023)

### Front-end / language

- C++23 mode (`-std=c++23`) usable for general code (still experimental).
- New `[[assume(expr)]]` attribute (C++23) — see
  [features/std/cpp23/cpp23_assume.cpp](../features/std/cpp23/cpp23_assume.cpp).
- `static operator()` and `static operator[]` (C++23) — see
  [features/std/cpp23/cpp23_static_operator.cpp](../features/std/cpp23/cpp23_static_operator.cpp).
- `consteval` propagation per P2564.
- C2X / C23 progress; `_BitInt(N)` bit-precise integer types in C.

### libstdc++

- **`<format>`** ships with a usable implementation for the first time —
  see [features/std/cpp20/cpp20_format.cpp](../features/std/cpp20/cpp20_format.cpp) and
  [features/gcc/gcc13/gcc13_libstdcxx_format.cpp](../features/gcc/gcc13/gcc13_libstdcxx_format.cpp).
- C++23: `std::byteswap`, `std::to_underlying`, `string::contains`,
  `string_view::contains`, `views::zip`, `views::chunk`, `views::slide`,
  `views::enumerate`, `views::join_with`, `views::adjacent`,
  `views::adjacent_transform`.

### Optimization

- New IR-level vectorizer cost model improvements; SLP vectorizer can
  vectorize more patterns out of the box.
- LTO partitioning improvements.
- Improved devirtualization across LTO boundaries.

### Diagnostics & analyzer

- `-fanalyzer` (the static analyzer) gained many new checkers and
  significantly reduced false positives. C++ support, while still partial,
  improved a lot here.
- New `-Wenum-conversion` etc.

### Sanitizers

- `-fsanitize=shadow-call-stack` for AArch64.
- Improved diagnostics under UBSan / ASan.

### Target / codegen

- AMD Zen 4 (`-march=znver4`) tuning.
- LoongArch baseline expanded.
- `--enable-host-pie` build option.

Source: <https://gcc.gnu.org/gcc-13/changes.html>

---

## GCC 14 (April 2024)

### Front-end / language

- C++26 (`-std=c++26`) added as experimental.
- Saturation arithmetic helpers (`std::add_sat` etc.) usable from C++26 mode —
  see
  [features/std/cpp26/cpp26_saturation_arith.cpp](../features/std/cpp26/cpp26_saturation_arith.cpp).
- `[[indeterminate]]` attribute progress.
- C23 default for C front-end gets closer.

### libstdc++

- **`std::ranges::to`** moved into the main library — see
  [features/std/cpp23/cpp23_ranges_to.cpp](../features/std/cpp23/cpp23_ranges_to.cpp) and
  [features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp](../features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp).
- **`std::generator`** (C++23) — coroutine-backed lazy generator. See
  [features/std/cpp23/cpp23_generator.cpp](../features/std/cpp23/cpp23_generator.cpp).
- **`std::mdspan`** (C++23) — non-owning multi-dimensional view. See
  [features/std/cpp23/cpp23_mdspan.cpp](../features/std/cpp23/cpp23_mdspan.cpp).
- **`std::print` / `std::println`** (C++23) — formatted stdout output, but the
  implementation was still in the *experimental* library on GCC 14
  (`-lstdc++exp`). See
  [features/std/cpp23/cpp23_print.cpp](../features/std/cpp23/cpp23_print.cpp).
- **`std::stacktrace`** (C++23) — also in libstdc++exp on GCC 14
  (`-lstdc++exp`). See
  [features/std/cpp23/cpp23_stacktrace.cpp](../features/std/cpp23/cpp23_stacktrace.cpp).
- `std::flat_map` / `std::flat_set` (C++23) shipped with rough edges.
- `views::cartesian_product`, `views::repeat`, `views::stride`.

### Optimization

- New `-fhardened` umbrella option that turns on a curated set of hardening
  flags (`-D_FORTIFY_SOURCE`, `-fstack-protector-strong`,
  `-fstack-clash-protection`, `-fcf-protection=full`, etc.).
- Improved `-fcoroutines` codegen.
- More aggressive auto-vectorisation: opportunities at `-O2` widened.

### Diagnostics & analyzer

- Diagnostic `-Wnrvo` warns on missed Named Return Value Optimization.
- `-fanalyzer` now understands more standard-library functions; better C++
  support.

### Sanitizers

- ASan and UBSan get faster runtime checks; reduced false positives.

### Target / codegen

- AArch64 SME (Scalable Matrix Extension) progress.
- Intel APX support added.
- AMDGPU offloading via OpenMP improved.

Source: <https://gcc.gnu.org/gcc-14/changes.html>

---

## GCC 15 (April 2025)

### Front-end / language

- About two-thirds of C++26 features available with `-std=c++26`.
- Default C standard bumped to **C23**.
- COBOL front-end added.
- More C++26 paper implementations: contracts framework progress, parts of
  reflection (P2996) appearing behind switches.

### libstdc++

- **`std::print` / `std::println` are now in the main libstdc++** — no more
  `-lstdc++exp` for the common case. See
  [features/gcc/gcc15/gcc15_default_print.cpp](../features/gcc/gcc15/gcc15_default_print.cpp).
- `std::flat_map` / `std::flat_set` matured; bugs from GCC 14 cleaned up.
- More C++26 library features land.

### Optimization

- LTO performance and memory usage further improved.
- Better vectorization for short loops with reductions.

### Diagnostics & analyzer

- More analyzer checkers stabilise; C++ analyzer support broader.
- Enhanced `-Wuninitialized` heuristics.

### Sanitizers

- New runtime checks; improved interaction with optimization.
- Memory tagging on AArch64 (HWASAN) gets a usability bump.

### Target / codegen

- Default Intel architecture target bumped (configure-time choice).
- Continued AArch64 SVE2 / SME polish.

Source: <https://gcc.gnu.org/gcc-15/changes.html>

---

## GCC 16 (April 2026)

### Front-end / language

- More C++26 maturation: `std::execution` sender/receiver async framework,
  contracts machinery, additional reflection bits.
- Algol 68 front-end added.

### libstdc++

- Continued ranges / views polish.
- Most C++26 library headers usable.

### Optimization

- Auto-vectorizer recognises more patterns; improved 512-bit codegen on
  AVX-512 / AVX10 capable cores.
- LTO continues to shrink memory + time.

### Target / codegen

- **Intel AVX10** support.
- Continued RISC-V vector / Zvk bump.

### Diagnostics & analyzer

- Reflection-aware diagnostics start to appear.
- More precise lifetime / dangling-pointer warnings.

Source: <https://gcc.gnu.org/gcc-16/changes.html>

---

## How this maps to the examples

| Release smoke-test                                                                  |
|--------------------------------------------------------------------------------------|
| [features/gcc/gcc13/gcc13_libstdcxx_format.cpp](../features/gcc/gcc13/gcc13_libstdcxx_format.cpp)        |
| [features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp](../features/gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp)  |
| [features/gcc/gcc15/gcc15_default_print.cpp](../features/gcc/gcc15/gcc15_default_print.cpp)              |
| [features/gcc/gcc16/gcc16_cpp26_features_default.cpp](../features/gcc/gcc16/gcc16_cpp26_features_default.cpp) |

For non-version-specific GCC features that are *always* there (attributes,
builtins, OpenMP, vector_size types, target multi-versioning, etc.), browse
the [gccext index](gccext.md).

## What's intentionally *not* benchmarked here

A few classes of compiler features can't be exercised through this repo's
"compile + run + assert exit code" harness:

- **Auto-vectorization / instruction selection.** We can compile with `-O3`
  and assert *correctness* (see
  [features/gccext/codegen/gccext_autovectorize.cpp](../features/gccext/codegen/gccext_autovectorize.cpp)),
  but verifying that a loop *actually* used `vfmadd231ps` requires reading
  assembly or `-fopt-info-vec`. Use the script's `--show-cmds`/`--verbose`
  to grab the exact build line, then re-run it locally with `-fopt-info-vec`
  or `objdump -d`.
- **LTO / PGO** improve speed without changing observable behaviour. Add
  `-flto` to a build command to test it; correctness assertions still hold.
- **`-fanalyzer` (the static analyzer)** emits warnings, not exit codes.
  Run it as a separate compile pass on any example to see what it flags.
- **Sanitizers** (`-fsanitize=address,undefined,thread`) abort at runtime
  on errors but don't change the *successful* path. A sanitizer CI row would
  re-run every example with the flags and fail on any new abort — listed as
  a future enhancement in [../README.md](../README.md).
