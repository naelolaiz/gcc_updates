# Default-behaviour changes across GCC releases

What changes between GCC releases when you pass **no** flags (or only `-O2`)?
Everything below was measured inside the same containers CI uses — the
official `gcc:{13,14,15}` images plus Debian unstable's `g++-16` — with
`g++ -E -dM` for predefined macros and `g++ -O2 -Q --help=optimizers` /
`--help=common` diffs for flag defaults. Runnable companions live in
[features/gcc/defaults/](../features/gcc/defaults/).

## Default language dialect

The dialect you get with no `-std` flag (always the GNU flavour, never
strict ISO):

| Release | C++ default (`__cplusplus`) | C default (`__STDC_VERSION__`) |
|---------|-----------------------------|--------------------------------|
| GCC 13  | gnu++17 (`201703L`) | gnu17 (`201710L`) |
| GCC 14  | gnu++17 (`201703L`) | gnu17 (`201710L`) |
| GCC 15  | gnu++17 (`201703L`) | **gnu23 (`202311L`)** ← changed |
| GCC 16  | **gnu++20 (`202002L`)** ← changed | gnu23 (`202311L`) |

Demo: [gccdef_dialect.cpp](../features/gcc/defaults/gccdef_dialect.cpp)
(registered with `STD default`, i.e. compiled without `-std`).

This repo is immune by construction — every `gcc_feature_test()` pins its
`STD` — but any build system that omits `-std` silently moved from C++17 to
C++20 semantics when its toolchain reached GCC 16.

## Floating-point semantics: stable, and more permissive than you think

None of these changed between 13 and 16, which is its own finding — the
notable one being that **`-ffp-contract=fast` is the default even in strict
`-std=c++NN` modes**, so `a*b + c` may compile to a single fused
multiply-add (one rounding instead of two) as soon as the target ISA has
FMA. Your numeric results can change by adding `-march=x86-64-v3`, without
any fast-math flag.

| Flag | Default in GCC 13–16 (any dialect, `-O2`) |
|------|-------------------------------------------|
| `-ffp-contract` | `fast` — contraction allowed everywhere |
| `-fexcess-precision` | `standard` semantics; moot on x86-64 SSE (`__FLT_EVAL_METHOD__ == 0`) |
| `-fmath-errno` | enabled — `sqrt(-1)` still sets `errno` |
| `-ftrapping-math`, `-fsigned-zeros` | enabled — IEEE traps/zero signs respected |
| `-fassociative-math`, `-freciprocal-math`, `-ffinite-math-only`, `-funsafe-math-optimizations` | disabled |

Demo: [gccdef_fp_contract.cpp](../features/gcc/defaults/gccdef_fp_contract.cpp)
observes the fused result at runtime (built with `-mfma`); build with
`-ffp-contract=off` when two-rounding reproducibility matters.

## New optimizations enabled by default at `-O2`

From `g++ -O2 -Q --help=optimizers` diffs between consecutive releases
(new flags that arrive already enabled):

| First release | Enabled at `-O2` | What it does |
|---------------|------------------|--------------|
| GCC 14 | `-ffold-mem-offsets` | folds address offsets into memory instructions |
| GCC 15 | `-fext-dce` | dead-code-eliminates redundant sign/zero extensions |
| GCC 15 | `-flate-combine-instructions` | extra instruction-combining pass late in RTL |
| GCC 15 | `-fmalloc-dce` | removes malloc/free pairs whose memory is never used |
| GCC 15 | `-foptimize-crc` | recognises CRC loops and emits carry-less multiply |
| GCC 16 | `-fdep-fusion` | fuses dependent instructions |
| GCC 16 | `-fspeculatively-call-stored-functions` | speculates indirect calls through stored function pointers |
| GCC 16 | `-fauto-profile-inlining` | inlining driven by AutoFDO profiles when present |

These are behaviour-preserving, so there is no runnable demo — the point is
that plain `-O2` gets measurably smarter each release without any flag
churn on your side.

## Packaging defaults ≠ upstream defaults

The official `gcc:N` images are upstream builds: **non-PIE** by default.
Debian's `g++-16` package (what the CI gcc-16 and analyzer jobs use)
enables **`-fPIE`/`-fPIC` by default** as part of distro hardening. Same
compiler version, different binaries out of the box.

Demo: [gccdef_pie_default.cpp](../features/gcc/defaults/gccdef_pie_default.cpp)
prints `__PIE__`/`__PIC__` and asserts the one invariant that survives
packaging (PIE ⇒ PIC). Moral: when comparing toolchains, compare the
*build* of the compiler, not just its version number.

## New diagnostics per release

Each release adds warnings. One representative new warning per release has a
runnable demo in its `features/gcc/gccNN/` bucket (topic `gcc-diagnostics`,
run them all with `ctest -L gcc-diagnostics`); each demo compiles a
deliberately flawed file with `-Werror=<warning>` and asserts the exact
diagnostic fires:

| Introduced | Warning | Demo |
|------------|---------|------|
| GCC 13 | `-Wdangling-reference` — reference bound to a temporary through a call | [gcc13_warn_dangling_reference.cpp](../features/gcc/gcc13/gcc13_warn_dangling_reference.cpp) |
| GCC 14 | `-Wcalloc-transposed-args` — `calloc(sizeof(T), n)` arguments backwards | [gcc14_warn_calloc_transposed.cpp](../features/gcc/gcc14/gcc14_warn_calloc_transposed.cpp) |
| GCC 15 | `-Wdeprecated-literal-operator` — C++23-deprecated `operator "" _x` spelling | [gcc15_warn_deprecated_literal_operator.cpp](../features/gcc/gcc15/gcc15_warn_deprecated_literal_operator.cpp) |
| GCC 16 | `-Wc++26-compat` — identifiers (e.g. `contract_assert`) becoming C++26 keywords | [gcc16_warn_cpp26_compat.cpp](../features/gcc/gcc16/gcc16_warn_cpp26_compat.cpp) |

Other default-enabled additions (GCC 14: `-Whardened`, several
`-Wanalyzer-*`; GCC 15: `-Wmusttail-local-addr`, diagnostics event
links/colours; GCC 16: `-Wanalyzer-throw-of-unexpected-type`) only matter for
builds already opting into the relevant feature, so they are listed for
completeness rather than demoed.
