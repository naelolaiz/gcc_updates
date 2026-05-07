# Compiler flags reference

This file documents *exactly* how every example in [features/](../features/) is
built. The build engine is [scripts/discover.py](../scripts/discover.py); this
doc is the human-readable spec of what it does so you can reproduce any build
by hand.

## Quick lookup: print the command for any example

```bash
./scripts/podman-dev.sh 15 --show-cmds | grep -A1 cpp23_print
```

`--show-cmds` walks every `.cpp`, derives its full build command from the
metadata header, and prints it without compiling anything. Use it whenever you
want to know *exactly* how a single example would be built.

`--verbose` prints the same command immediately *before* each actual compile
when running tests, so the CI logs always show the literal `g++ …` line that
produced the result.

## Default command

Every example gets this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -O2 -pthread -Ifeatures \
    features/<bucket>/<file>.cpp \
    $EXTRA_FLAGS \
    -o /tmp/gcc_updates_build/<file>
```

`<bucket>` is `std/cppNN`, `gcc/gccNN`, or `gccext/<topic>` — whichever
subfolder the example lives in. The engine derives it from the file path.

Where:

| Token        | Source                          | Why                                                                             |
|--------------|---------------------------------|---------------------------------------------------------------------------------|
| `g++`        | active toolchain                | The `g++` already on `$PATH` inside the container. Matrix and sanitizer jobs run in `gcc:N`, where `g++` is the default. The analyze job runs in `debian:unstable-slim` and installs `g++-16`, then points `g++` at it via `update-alternatives`. |
| `-std=$STD`  | `// gcc-test: std=…`            | Selects C++ standard. Allowed: `c++11`, `c++14`, `c++17`, `c++20`, `c++23`, `c++26`. (`c++14` is recognised but not currently used; the cpp11 bucket is built with `c++11`.) |
| `-Wall -Wextra -Wpedantic` | hard-coded default | Strict warnings — examples must compile clean.                                  |
| `-O2`        | hard-coded default              | Realistic optimisation level — catches subtle UB the inliner exposes.           |
| `-pthread`   | hard-coded default              | Always on. No-op for non-threading code; required for `<thread>`, `<atomic>`'s wait/notify, semaphores, latches, etc. |
| `-Ifeatures` | hard-coded default              | Lets examples include the shared readable-demo helper as `support/demo.hpp`.    |
| `$EXTRA_FLAGS` | `// gcc-test: extra-flags=A,B` | Comma-separated, file-specific flags (e.g. linker libs). Empty for most files.  |

The output binary lives in `/tmp/gcc_updates_build/`, runs once, and the
build dir is wiped between invocations.

## Per-file `extra-flags=` values you'll see in this repo

| Flag           | Used by which examples                                          | What it does                                                                                                                                                              |
|----------------|------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `-lstdc++exp`  | `std/cpp23/cpp23_print.cpp`, `std/cpp23/cpp23_stacktrace.cpp`    | Links libstdc++exp (the *experimental* libstdc++ archive). Required on **GCC 14** for `std::print`/`std::println`/`std::stacktrace`. **GCC 15+ doesn't need it** — see [gcc/gcc15/gcc15_default_print.cpp](../features/gcc/gcc15/gcc15_default_print.cpp). |
| `-ltbb`        | `std/cpp17/cpp17_parallel_algos.cpp`                             | Links Intel TBB. Parallel STL execution policies (`std::execution::par`, `par_unseq`) are implemented on top of TBB on libstdc++. Without this you get linker errors for `__pstl_*` symbols. |
| `-fopenmp`     | `gccext/openmp/gccext_openmp_parallel_for.cpp`                   | Enables OpenMP pragmas AND links libgomp at the same time. Single flag for both compile and link.                                                                          |
| `-fcontracts`  | `std/cpp26/cpp26_contracts_basic.cpp`                            | Enables C++26 contracts machinery (experimental in GCC 15+).                                                                                                              |
| `-freflection` | `std/cpp26/cpp26_reflection_basic.cpp`                           | Enables C++26 static-reflection front-end (experimental in GCC 16+).                                                                                                       |

## How to read a `// gcc-test:` header

Every example starts with a line shaped like:

```cpp
// gcc-test: std=c++23 min-gcc=14 topic=stl experimental=false extra-flags=-lstdc++exp
```

| Key            | Required? | Meaning                                                                                              |
|----------------|-----------|------------------------------------------------------------------------------------------------------|
| `std=`         | yes       | Passed to `-std=`. One of `c++11`,`c++14`,`c++17`,`c++20`,`c++23`,`c++26`.                            |
| `min-gcc=`     | yes       | Skip on compilers older than this major version.                                                     |
| `topic=`       | yes       | Free-form short label used to group examples in the docs index.                                      |
| `experimental=`| yes       | If `true`, the file is allowed to fail — but only in the *expected* way. Requires `expect-error=` (a regex matched against stderr). A failure that matches the regex is reported as yellow `EXP-FAIL` and tolerated. A failure that does **not** match is a red `EXP-WRONG-ERR` and hard-fails CI. A *successful* build of an experimental file hard-fails as `EXP-PROMOTE` — flip `experimental=false` and drop the regex. |
| `extra-flags=` | no        | Comma-separated. Each token is appended after the source file so linker libraries resolve symbols. Use comma, not space. |
| `run-args=`    | no        | Shell-tokenised argv passed to the compiled binary at runtime.                                       |
| `expect-exit=` | no        | Compare against this exit code (default `0`).                                                        |
| `max-gcc=`     | no        | Skip on compilers *newer* than this. Useful for examples that demonstrate a since-removed behaviour. |
| `requires-sanitizer=` | no | Comma-separated sanitizer names (e.g. `undefined`, `address`, `thread`, `leak`). The example only runs when `discover.py --sanitize=…` includes one of them — typical for files that *deliberately* trigger UB so the assertion is "trips the right kind of check". See [../features/gccext/sanitize/README.md](../features/gccext/sanitize/README.md). |
| `requires-analyzer=` | no | If `true`, the example only runs in `--analyzer` mode and is **compile-only** (its binary contains UB you don't want to execute). See [../features/gccext/analyzer/README.md](../features/gccext/analyzer/README.md). |
| `expect-error=` | required when `experimental=true` | A Python regex matched against the failing build's combined stdout+stderr. Lets the harness distinguish "feature legitimately not yet shipped" (matches → soft-pass) from "the test is broken in some unrelated way" (no match → hard fail). Quote the value if it contains spaces or pipes: `expect-error="(foo\|bar). is not a member of"`. |
| `skip-sanitizer=` | no | Comma-separated list. Skip the file when any of these sanitizers is active. Use for examples whose data race lives **inside** an instrumented runtime (libgomp, IFUNC dispatch) rather than in the example itself — a TSan complaint there is noise, not a real signal. |
| `min-libstdcxx=` | no | Require `_GLIBCXX_RELEASE >= N`. Independent of `min-gcc=`: g++ and libstdc++ are versioned separately, so a feature gated on the runtime library has to be checked separately. Files using a libstdc++-only symbol (e.g. `std::ranges::starts_with`, libstdc++ 16+ per cppreference) should set this to the libstdc++ release that actually ships it. |
| `max-libstdcxx=` | no | Symmetric upper bound. Use for behaviour that was removed/changed in a later libstdc++. |

The libstdc++ release is detected once at startup by asking g++ to preprocess a
tiny translation unit that includes `<version>` and reports the `_GLIBCXX_RELEASE`
macro (no compile, no run).
The detected value is printed in the run configuration block at the start of
every run. If the macro is unavailable (very old toolchain),
`min-libstdcxx=` gates are not enforced.

## Reproducing a build by hand

Inside the container:

```bash
# Drop into a shell (the local image scripts/podman-dev.sh builds is
# FROM gcc:${GCC_VERSION} + libtbb-dev + python3).
podman run --rm -it -v "$(pwd):/work:rw,Z" -w /work \
    localhost/gcc-updates:gcc15 bash

# Inside the container, run the example's command (copy from --show-cmds output)
g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 -pthread -Ifeatures \
    features/std/cpp23/cpp23_stacktrace.cpp -lstdc++exp -o /tmp/stacktrace
/tmp/stacktrace
```

The container is the only supported way to invoke the toolchain locally —
the host shell is off-limits for compilation.

## Adding a new flag

1. If a new example needs a flag, add it as `extra-flags=…` in *that file's*
   metadata header.
2. If it's needed for *every* example, add it to `DEFAULT_FLAGS` in
   [scripts/discover.py](../scripts/discover.py) AND list it in the
   "Default command" table above.
3. If it requires a new package in the build environment, install it in BOTH
   [containers/gcc.Containerfile](../containers/gcc.Containerfile) and
   [.github/workflows/ci.yml](../.github/workflows/ci.yml). They must stay in
   sync — both layer on top of the official `gcc:N` Docker image.
