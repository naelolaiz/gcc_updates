# Sanitizers

This repo runs every example under sanitizers as a separate CI row, so any
latent UB / leak / data race in an example is caught the same day it lands.

## TL;DR

```bash
# Locally: re-build everything under UBSan + ASan in the container.
./scripts/container-dev.sh 15 sanitize=undefined,address

# Same proofs under clang's compiler-rt runtimes (the upstream original;
# GCC's libsanitizer is a port of it):
./scripts/container-dev.sh clang22 sanitize=undefined,address
./scripts/container-dev.sh clang22 sanitize=thread

# CI does the equivalent on dedicated jobs (see .github/workflows/ci.yml).
```

CI runs each sanitizer combination twice: once under GCC 15 (libsanitizer)
and once under clang 22 (compiler-rt), so a deliberate-trip demo must produce
its declared report under both instrumentations. The one known divergence is
gated: clang's `vptr` check crashes with a SEGV on a null vtable pointer
instead of reporting, so
[gccext_ubsan_invalid_vptr](../features/gccext/sanitize/ubsan/gccext_ubsan_invalid_vptr.cpp)
is `GCC_ONLY`.

If any example aborts with a sanitizer report, that run fails. Examples are
expected to be sanitizer-clean; if a deliberate UB demo is needed, opt out
with `[[gnu::no_sanitize("...")]]` (see
[../features/gccext/sanitize/integration/gccext_no_sanitize_attribute.cpp](../features/gccext/sanitize/integration/gccext_no_sanitize_attribute.cpp)).

Files that *deliberately* trip a sanitizer (e.g. a heap-use-after-free demo)
live under [../features/gccext/sanitize/{asan,ubsan,tsan,leak}/](../features/gccext/sanitize/)
and use `REQUIRES_SANITIZER` in their `gcc_feature_test()` call so CTest
enables them **only** under the matching sanitizer. They use `WILL_FAIL` plus
`EXPECT_OUTPUT` so an unrelated crash or loader error does not pass as the
expected sanitizer report.

## What `-DGCC_FEATURE_SANITIZE=…` adds to the build

Whatever you pass becomes `-fsanitize=<list>`, plus three always-on companions
(and `-O2` is dropped to `-O1` so the optimiser doesn't elide deliberate UB
in asan/ubsan demos):

```
-fsanitize=<list>
-O1                            # was -O2; -O2 can elide deliberate out-of-bounds stores
-fno-omit-frame-pointer        # readable backtraces in reports
-g                             # source-level locations in reports
```

`<list>` is a comma-separated subset of the GCC sanitizer names. Common
groupings:

| Token | What it catches | Compatible with |
|-------|-----------------|------------------|
| `undefined` | Signed overflow, OOB, null deref, misaligned access, shift UB, divide-by-zero, … | everything else |
| `address` (ASan) | Heap/stack/global out-of-bounds, use-after-free, double-free | UBSan; **mutually exclusive** with TSan and MSan |
| `thread` (TSan) | Data races between threads | UBSan; mutually exclusive with ASan/MSan |
| `leak` | Memory leaks at exit | implied by ASan |
| `pointer-compare` / `pointer-subtract` | Comparing pointers from unrelated objects | ASan |
| `vptr` | Bad virtual table pointer | UBSan |

The default sanitizer row in CI uses `undefined,address`, which is the
combination most useful for everyday correctness checks.

## Per-function opt-out

Sometimes a hot path is provably safe and the runtime check has measurable
cost. Mark just that function:

```cpp
[[gnu::no_sanitize("undefined")]]
unsigned long long fast(unsigned long long a, unsigned long long b) {
    return a + b;          // no UBSan instrumentation here
}
```

Multiple checks can be listed: `[[gnu::no_sanitize("address", "undefined")]]`.

## Other "sanitizer-adjacent" tools available in this repo

- **`-D_GLIBCXX_DEBUG`** — libstdc++'s debug mode. Flags iterator misuse,
  out-of-range access, container/iterator mismatches at runtime. Demo:
  [../features/gccext/sanitize/integration/gccext_glibcxx_debug.cpp](../features/gccext/sanitize/integration/gccext_glibcxx_debug.cpp).
- **`[[assume(expr)]]`** — under `-fsanitize=undefined`, the assumption is
  *checked* at runtime; otherwise it's a pure optimisation hint. Demo:
  [../features/gccext/sanitize/integration/gccext_assume_under_sanitize.cpp](../features/gccext/sanitize/integration/gccext_assume_under_sanitize.cpp).
- **`-fanalyzer`** — GCC's static analyzer. Compile-time only; emits
  `-Wanalyzer-*` diagnostics (path-sensitive) instead of trapping at runtime,
  so it catches paths an actual test input may never reach. The
  [../features/gccext/analyzer/](../features/gccext/analyzer/) bucket has
  four demos (use-after-free, conditional double-free, a lost allocation, and
  path-sensitive null dereference). CI's `analyze` job compiles them at `-O0`
  on GCC 16 and requires each declared `-Wanalyzer-*` category to appear;
  this is a checked test, not merely a diagnostic left in the build log. For
  ad-hoc runs:
  `./scripts/container-dev.sh 16 analyzer`.

## Runtime knobs

Sanitizers honour environment variables for behaviour tuning:

```bash
ASAN_OPTIONS=detect_stack_use_after_return=1:halt_on_error=1
UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1
```

The CI job sets `halt_on_error=1` so the first sanitizer report fails the
build immediately rather than letting the program continue.

## Why some sanitizers aren't combined

ASan, TSan, and MSan all rewrite memory access — only one can be active at a
time. `undefined` is a different mechanism (per-operation checks at the
instruction level) and composes with any of them. So:

- ✅ `undefined,address` — the default in this repo.
- ✅ `undefined,thread`  — for thread-correctness focus.
- ❌ `address,thread`   — incompatible.
