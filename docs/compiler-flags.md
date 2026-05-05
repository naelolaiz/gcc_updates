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
g++ -std=$STD -Wall -Wextra -Wpedantic -O2 -pthread $EXTRA_FLAGS \
    features/<bucket>/<file>.cpp \
    -o /tmp/gcc_updates_build/<file>
```

`<bucket>` is `std/cppNN`, `gcc/gccNN`, or `gccext/<topic>` — whichever
subfolder the example lives in. The engine derives it from the file path.

Where:

| Token        | Source                          | Why                                                                             |
|--------------|---------------------------------|---------------------------------------------------------------------------------|
| `g++`        | active toolchain                | Resolved by `update-alternatives` to `g++-13/14/15/16` inside the container.    |
| `-std=$STD`  | `// gcc-test: std=…`            | Selects C++ standard. Allowed: `c++17`, `c++20`, `c++23`, `c++26` (also c++11/c++14 reserved for the C++11 set). |
| `-Wall -Wextra -Wpedantic` | hard-coded default | Strict warnings — examples must compile clean.                                  |
| `-O2`        | hard-coded default              | Realistic optimisation level — catches subtle UB the inliner exposes.           |
| `-pthread`   | hard-coded default              | Always on. No-op for non-threading code; required for `<thread>`, `<atomic>`'s wait/notify, semaphores, latches, etc. |
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
| `experimental=`| yes       | If `true`, a failure is reported in yellow ("EXP-FAIL") but does **not** fail the matrix row.        |
| `extra-flags=` | no        | Comma-separated. Each token is appended verbatim to `g++ …`. Use comma, not space.                   |
| `run-args=`    | no        | Shell-tokenised argv passed to the compiled binary at runtime.                                       |
| `expect-exit=` | no        | Compare against this exit code (default `0`).                                                        |
| `max-gcc=`     | no        | Skip on compilers *newer* than this. Useful for examples that demonstrate a since-removed behaviour. |

## Reproducing a build by hand

Inside the container:

```bash
# Drop into a shell
podman run --rm -it -v "$(pwd):/work:rw,Z" -w /work \
    localhost/gcc-updates:gcc15 bash

# Inside the container, run the example's command (copy from --show-cmds output)
g++ -std=c++23 -Wall -Wextra -Wpedantic -O2 -pthread -lstdc++exp \
    features/std/cpp23/cpp23_stacktrace.cpp -o /tmp/stacktrace
/tmp/stacktrace
```

The container is the only supported way to invoke the toolchain locally —
the host shell is off-limits for compilation.

## Adding a new flag

1. If a new example needs a flag, add it as `extra-flags=…` in *that file's*
   metadata header.
2. If it's needed for *every* example (rare — `-pthread` is the only such),
   add it to `DEFAULT_FLAGS` in [scripts/discover.py](../scripts/discover.py)
   AND list it in the "Default command" table above.
3. If it requires a new package in the build environment, install it in BOTH
   [containers/gcc.Containerfile](../containers/gcc.Containerfile) and
   [.github/workflows/ci.yml](../.github/workflows/ci.yml). They must stay in
   sync — that's the whole point of the toolchain-PPA-everywhere design.
