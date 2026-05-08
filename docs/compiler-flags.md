# Compiler flags reference

This file documents *exactly* how every example in [features/](../features/)
is built. The build is driven by CMake; the function macro is defined in
[../cmake/GccFeature.cmake](../cmake/GccFeature.cmake), and each example is
registered via one `gcc_feature_test()` call in its folder's `CMakeLists.txt`.

## Quick lookup: print the command for any example

```bash
# Configure once, then dump the compile command for any target.
cmake -S . -B build
cmake --build build --target cpp23_print --verbose 2>&1 | grep g++
```

## Default command

Every example gets this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -O2 -pthread -Ifeatures \
    features/<bucket>/<file>.cpp \
    $EXTRA_COMPILE_FLAGS \
    -o build/features/<bucket>/<file>
```

`<bucket>` is `std/cppNN`, `gcc/gccNN`, or `gccext/<topic>` — whichever
subfolder the example lives in.

| Token        | Source                          | Why                                                                             |
|--------------|---------------------------------|---------------------------------------------------------------------------------|
| `g++`        | active toolchain                | The `g++` already on `$PATH` inside the container. Matrix and sanitizer jobs run in `gcc:N`, where `g++` is the default. The analyze job runs in `debian:unstable-slim` and installs `g++-16`, then points `g++` at it via `update-alternatives`. |
| `-std=$STD`  | `STD` arg of `gcc_feature_test()` | Selects C++ standard. Allowed: `c++11`, `c++14`, `c++17`, `c++20`, `c++23`, `c++26`. |
| `-Wall -Wextra -Wpedantic` | hard-coded default | Strict warnings — examples must compile clean.                                  |
| `-O2`        | hard-coded default              | Realistic optimisation level — catches subtle UB the inliner exposes. **Sanitizer mode lowers this to `-O1`** to keep deliberate UB observable. |
| `-pthread`   | hard-coded default              | Always on. No-op for non-threading code; required for `<thread>`, `<atomic>`'s wait/notify, semaphores, latches, etc. |
| `-Ifeatures` | hard-coded default              | Lets examples include the shared readable-demo helper as `support/demo.hpp`.    |
| `$EXTRA_COMPILE_FLAGS` | `EXTRA_COMPILE_FLAGS …` arg | File-specific flags (e.g. `-fopenmp`, `-D_GLIBCXX_DEBUG`, `-O3`). |

The output binary lives in `build/features/<bucket>/<file>` (CMake's standard
out-of-source build layout). `EXTRA_LIBS` gets added via
`target_link_libraries(... PRIVATE …)` after the source file, e.g. `stdc++exp`
for `std::stacktrace` or `tbb` for parallel STL.

## Per-test arguments you'll see in this repo

| Argument            | Used by which examples                                | What it does |
|---------------------|-------------------------------------------------------|--------------|
| `EXTRA_LIBS stdc++exp` | `std/cpp23/cpp23_print.cpp`, `std/cpp23/cpp23_stacktrace.cpp` | Links libstdc++exp (the *experimental* libstdc++ archive). Required on **GCC 14** for `std::print`/`std::println`/`std::stacktrace`. **GCC 15+** ships them in the main libstdc++ — see [gcc/gcc15/gcc15_default_print.cpp](../features/gcc/gcc15/gcc15_default_print.cpp). |
| `EXTRA_LIBS tbb`    | `std/cpp17/cpp17_parallel_algos.cpp`                  | Links Intel TBB. Parallel STL execution policies (`std::execution::par`, `par_unseq`) are implemented on top of TBB on libstdc++. |
| `EXTRA_COMPILE_FLAGS -fopenmp` | `gccext/openmp/gccext_openmp_parallel_for.cpp` | Enables OpenMP pragmas. The function automatically forwards `-fopenmp` to the link step too so libgomp is linked. |
| `EXTRA_COMPILE_FLAGS -fcontracts` | `std/cpp26/cpp26_contracts_basic.cpp` | Enables C++26 contracts (currently fails to compile — `EXPERIMENTAL` + `EXPECT_ERROR` makes that the expected outcome). |
| `EXTRA_COMPILE_FLAGS -freflection` | `std/cpp26/cpp26_reflection_basic.cpp` | Enables C++26 static-reflection front-end (GCC 16+). |
| `EXTRA_COMPILE_FLAGS -D_GLIBCXX_DEBUG` | `gccext/sanitize/integration/gccext_glibcxx_debug.cpp` | Turns on libstdc++ debug-mode containers + iterator checks. |
| `EXTRA_COMPILE_FLAGS -O3 -fopt-info-vec=…` | `gccext/codegen/gccext_autovectorize.cpp` | Bumps optimisation and asks GCC to report which loops vectorised. |

## How to read a `gcc_feature_test()` call

A typical entry in `features/std/cpp23/CMakeLists.txt`:

```cmake
gcc_feature_test(cpp23_stacktrace  STD c++23  MIN_GCC 14  TOPIC stl
                 EXTRA_LIBS stdc++exp  MIN_LIBSTDCXX 14)
```

| Argument            | Required? | Meaning |
|---------------------|-----------|---------|
| `<name>`            | yes       | Matches `<name>.cpp` in the same folder. |
| `STD <std>`         | yes       | Passed to `-std=`. One of `c++11`, `c++14`, `c++17`, `c++20`, `c++23`, `c++26`. |
| `MIN_GCC <n>`       | yes       | Tests on older GCC are not registered in that configure mode. |
| `TOPIC <name>`      | yes       | Becomes the CTest label — filter with `ctest -L <topic>`. |
| `MAX_GCC <n>`       | no        | Symmetric upper bound; useful for since-removed behaviour. |
| `MIN_LIBSTDCXX <n>` | no        | Require `_GLIBCXX_RELEASE >= n`. Independent of `MIN_GCC`. |
| `MAX_LIBSTDCXX <n>` | no        | Symmetric upper bound. |
| `EXTRA_LIBS <lib…>` | no        | Linker libraries (without `-l`). Added via `target_link_libraries`. |
| `EXTRA_COMPILE_FLAGS <flag…>` | no | Per-test compile flags (e.g. `-fopenmp`, `-D_GLIBCXX_DEBUG`). |
| `REQUIRES_SANITIZER <san…>` | no | Test enabled only when `-DGCC_FEATURE_SANITIZE=` includes one of the listed sanitizers. |
| `SKIP_SANITIZER <san…>` | no | Test disabled when active sanitizer matches (e.g. TSan + OpenMP is unsupported). |
| `REQUIRES_ANALYZER` | no        | Test only enabled with `-DGCC_FEATURE_ANALYZER=ON`. Compile-only with `-fanalyzer` — binary not run. |
| `WILL_FAIL`         | no        | Runtime test is expected to fail; requires `EXPECT_OUTPUT` so unrelated failures do not pass. |
| `EXPECT_OUTPUT <re>` | required with `WILL_FAIL` | Output regex that must match the expected sanitizer report text. |
| `EXPERIMENTAL`      | no        | Feature not yet supported by current GCC; combined with `EXPECT_ERROR`, the expected outcome is a compile failure. |
| `EXPECT_ERROR <re>` | required with `EXPERIMENTAL` | Output regex that must match the expected compiler diagnostic. Compilation success or an unrelated diagnostic fails the test. |

The libstdc++ release is detected at configure time by asking `g++` to
preprocess a tiny translation unit that includes `<version>` and reports
`_GLIBCXX_RELEASE`. The detected value is printed by `cmake -S . -B build`
and cached in `LIBSTDCXX_RELEASE`. If the macro is unavailable (very old
toolchain), `MIN_LIBSTDCXX` gates are not enforced.

## Reproducing a build by hand

```bash
# Build everything (or one target) inside the container. Working tree is
# read-only; the build directory lives in a podman named volume.
./scripts/podman-dev.sh 15                            # verbose CTest by default
./scripts/podman-dev.sh 15 -- -R cpp23_stacktrace     # one test, still verbose
./scripts/podman-dev.sh 15 -- -j                      # parallel CTest when speed matters
```

The container is the only supported way to invoke the toolchain locally —
the host shell is off-limits for compilation.

## Adding a new flag

1. If a new example needs a flag, add it as `EXTRA_COMPILE_FLAGS …` (or
   `EXTRA_LIBS …` for `-l<lib>`) on its `gcc_feature_test()` call.
2. If it's needed for *every* example, add it to `_gcc_feature_default_flags`
   in [../cmake/GccFeature.cmake](../cmake/GccFeature.cmake) AND list it
   in the "Default command" table above.
3. If it requires a new package in the build environment, install it in BOTH
   [../containers/gcc.Containerfile](../containers/gcc.Containerfile) and
   [../.github/workflows/ci.yml](../.github/workflows/ci.yml). They must
   stay in sync — both layer on top of the official `gcc:N` Docker image.
