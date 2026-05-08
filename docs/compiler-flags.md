# Compiler flags reference

This file documents *exactly* how every example in [features/](../features/)
is built. The build is driven by CMake; each example is declared in plain
CMake from its folder's `CMakeLists.txt.in` template, which the root
[../CMakeLists.txt](../CMakeLists.txt) configures into the build directory.
The build-dir copy at `build/features/<bucket>/CMakeLists.txt` is the
authoritative, fully-resolved description of what gcc is invoked with.

## Quick lookup: the resolved CMakeLists per leaf

```bash
# Configure once, then read the leaf's resolved CMakeLists. Every gcc flag
# that affects the example is inline in this file -- no helper to follow.
cmake -S . -B build
cat build/features/std/cpp23/CMakeLists.txt   # for any cpp23_* example

# Or get the literal compile + link command from the build system:
cmake --build build --target cpp23_print --verbose 2>&1 | grep g++
```

CI uploads the resolved tree as a workflow artifact named
`resolved-cmakelists-<mode>` (one per default / sanitize-asan-ubsan /
sanitize-tsan / analyzer job), so you can download it without rebuilding
locally.

## Default command

Every example gets this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -O2 -pthread -Ifeatures \
    features/<bucket>/<file>.cpp \
    [per-example extras] \
    -o build/features/<bucket>/<file>
```

`<bucket>` is `std/cppNN`, `gcc/gccNN`, or `gccext/<topic>` — whichever
subfolder the example lives in.

| Token        | Source                          | Why                                                                             |
|--------------|---------------------------------|---------------------------------------------------------------------------------|
| `g++`        | active toolchain                | The `g++` already on `$PATH` inside the container. Matrix and sanitizer jobs run in `gcc:N`, where `g++` is the default. The analyze job runs in `debian:unstable-slim` and installs `g++-16`, then points `g++` at it via `update-alternatives`. |
| `-std=$STD`  | leaf's `target_compile_options` | Selects C++ standard. Allowed: `c++11`, `c++14`, `c++17`, `c++20`, `c++23`, `c++26`. |
| `-Wall -Wextra -Wpedantic` | `GCC_FEATURE_DEFAULT_COMPILE_FLAGS` | Strict warnings — examples must compile clean.                                  |
| `-O2`        | `GCC_FEATURE_DEFAULT_COMPILE_FLAGS` | Realistic optimisation level. **Sanitizer mode swaps it for `-O1 -fno-omit-frame-pointer -g`** to keep deliberate UB observable. |
| `-pthread`   | `GCC_FEATURE_DEFAULT_COMPILE_FLAGS` | Always on. No-op for non-threading code; required for `<thread>`, `<atomic>`'s wait/notify, semaphores, latches, etc. |
| `-Ifeatures` | leaf's `target_include_directories` | Lets examples include the shared readable-demo helper as `support/demo.hpp`.    |
| per-example extras | leaf's `target_compile_options` (post-defaults) | `-fopenmp`, `-D_GLIBCXX_DEBUG`, `-O3`, `-fcontracts`, etc. — written inline in the leaf. |

The mode-specific flag lists are computed once in
[../cmake/GccFeature.cmake](../cmake/GccFeature.cmake) and substituted into
each leaf's `CMakeLists.txt.in` via configure_file's `@VAR@` mechanism:

| Variable                              | Holds                                                       |
|---------------------------------------|-------------------------------------------------------------|
| `GCC_FEATURE_DEFAULT_COMPILE_TEXT`    | `-Wall -Wextra -Wpedantic -O2 -pthread …` (with mode tweaks) |
| `GCC_FEATURE_SAN_COMPILE_TEXT`        | `-fsanitize=address -fsanitize=undefined …` when sanitize mode active, else empty |
| `GCC_FEATURE_SAN_LINK_TEXT`           | same `-fsanitize=…` for the link line                       |
| `GCC_FEATURE_ANALYZER_COMPILE_TEXT`   | `-fanalyzer` when analyzer mode active, else empty          |

After `configure_file()` runs at root configure time, the leaf's resolved
copy in the build dir contains the literal flag text inline — no `${VAR}`
references left to chase.

## Per-example extras you'll see in the leaves

| Pattern                                                              | Used by                                                          | Why |
|----------------------------------------------------------------------|------------------------------------------------------------------|-----|
| `target_link_libraries(<n> PRIVATE stdc++exp)`                       | `std/cpp23/cpp23_print`, `std/cpp23/cpp23_stacktrace`            | Links libstdc++exp (the experimental archive). Required on **GCC 14** for `std::print`/`println`/`stacktrace`. **GCC 15+** ships print/println in the main libstdc++ — see [features/gcc/gcc15/CMakeLists.txt.in](../features/gcc/gcc15/CMakeLists.txt.in). |
| `target_link_libraries(<n> PRIVATE tbb)`                             | `std/cpp17/cpp17_parallel_algos`                                 | Links Intel TBB. Parallel STL execution policies (`std::execution::par`) are implemented on top of TBB on libstdc++. |
| `target_compile_options(... -fopenmp)` + `target_link_options(... -fopenmp)` | `gccext/openmp/gccext_openmp_parallel_for`               | `-fopenmp` must be on BOTH compile and link (link-side flag is what pulls in libgomp). |
| `target_compile_options(... -fcontracts)`                            | `std/cpp26/cpp26_contracts_basic`                                | Enables C++26 contracts (currently fails to compile by design — `gcc_feature_expect_compile_error` makes that the expected outcome). |
| `target_compile_options(... -freflection)`                           | `std/cpp26/cpp26_reflection_basic`                               | Enables C++26 static-reflection front-end (GCC 16+). |
| `target_compile_options(... -D_GLIBCXX_DEBUG)`                       | `gccext/sanitize/integration/gccext_glibcxx_debug`               | Turns on libstdc++ debug-mode containers + iterator checks. |
| `target_compile_options(... -O3 -fopt-info-vec=…)`                   | `gccext/codegen/gccext_autovectorize`                            | Bumps optimisation and asks GCC to report which loops vectorised. |

## Version gates and sanitizer/analyzer mode gates

Each leaf writes its gates as plain CMake `if()` blocks. Examples:

```cmake
# Skip on GCC older than 15.
if(@GCC_MAJOR@ LESS 15)
    return()
endif()

# Skip when libstdc++ runtime is older than 13.
if(@LIBSTDCXX_RELEASE@ LESS 13)
    return()
endif()

# Sanitizer-required example: only build when address sanitizer is active.
if(NOT "address" IN_LIST GCC_FEATURE_ACTIVE_SANITIZERS)
    return()
endif()

# Skip the example when TSan is active (e.g. IFUNC + TSan races).
if("thread" IN_LIST GCC_FEATURE_ACTIVE_SANITIZERS)
    return()
endif()
```

The libstdc++ release is detected at configure time (preprocess `<version>`,
read `_GLIBCXX_RELEASE`); detection failure defaults `LIBSTDCXX_RELEASE` to
999 so MIN_LIBSTDCXX gates do not skip on unknown toolchains.

## Test-running helpers

The leaves call one of three helpers (defined in `cmake/GccFeature.cmake`)
to register the test. Each is a few lines long; the helper file is the
single place to look:

| Helper                                                  | When                                                                |
|---------------------------------------------------------|---------------------------------------------------------------------|
| `gcc_feature_normal_test(<name>)`                       | Default: run the binary, treat exit 0 as success. In analyzer mode the test is a no-op (the warning fires at compile time). |
| `gcc_feature_will_fail_test(<name> "<regex>")`          | Sanitizer demos: run the binary, expect non-zero exit, assert the diagnostic regex appears in stderr. |
| `gcc_feature_expect_compile_error(<name> <src> STD <std> EXTRA_FLAGS <flags...> REGEX "<regex>")` | EXPERIMENTAL features (e.g. C++26 contracts): no executable; instead invoke gcc directly and expect the listed diagnostic. |

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

1. If a new example needs a flag, add it inline to that example's
   `target_compile_options(...)` (or `target_link_libraries(...)`) call in
   the folder's `CMakeLists.txt.in`.
2. If it's needed for *every* example, add it to `GCC_FEATURE_DEFAULT_COMPILE_FLAGS`
   in [../cmake/GccFeature.cmake](../cmake/GccFeature.cmake) AND list it
   in the "Default command" table above.
3. If it requires a new package in the build environment, install it in BOTH
   [../containers/gcc.Containerfile](../containers/gcc.Containerfile) and
   [../.github/workflows/ci.yml](../.github/workflows/ci.yml). They must
   stay in sync — both layer on top of the official `gcc:N` Docker image.
