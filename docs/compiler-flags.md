# Compiler flags reference

This file documents *exactly* how every example in [features/](../features/)
is built. The build is driven by CMake; the function macro is defined in
[../cmake/GccFeature.cmake](../cmake/GccFeature.cmake), and each example is
registered via one `gcc_feature_test()` call in its folder's `CMakeLists.txt`.

## Quick lookup: print the command for any example

```bash
# Read one example and its registration, then run it in the supported container.
./scripts/container-dev.sh 16 show cpp23_print
./scripts/container-dev.sh 16 run cpp23_print
```

## Default command

Every example gets this baseline:

```
g++ -std=$STD -Wall -Wextra -Wpedantic -Werror -O2 -pthread -Ifeatures \
    features/<bucket>/<file>.cpp \
    $EXTRA_COMPILE_FLAGS \
    -o build/features/<bucket>/<file>
```

`<bucket>` is `std/cppNN`, `gcc/gccNN`, or `gccext/<topic>` — whichever
subfolder the example lives in.

| Token        | Source                          | Why                                                                             |
|--------------|---------------------------------|---------------------------------------------------------------------------------|
| `g++`        | active toolchain                | The `g++` already on `$PATH` inside the container. Matrix, sanitizer, and analyzer jobs run in the official `gcc:N` image, where `g++` is the default. The clang cross-check lane passes `-DCMAKE_CXX_COMPILER=clang++-22` instead; every other token stays identical. |
| `-std=$STD`  | `STD` arg of `gcc_feature_test()` | Selects C++ standard. Allowed: `c++11`, `c++14`, `c++17`, `c++20`, `c++23`, `c++26`, or `default` (omit `-std` entirely — used by `features/gcc/defaults/` to observe the compiler's default dialect). |
| `-Wall -Wextra -Wpedantic -Werror` | hard-coded default | Strict warnings — successful examples must compile clean. `WILL_FAIL` runtime probes and explicit `ALLOW_WARNINGS` registrations remove `-Werror`. |
| `-O2`        | hard-coded default              | Realistic optimisation level — catches subtle UB the inliner exposes. **Sanitizer mode lowers this to `-O1`** to keep deliberate UB observable. |
| `-pthread`   | hard-coded default              | Always on. No-op for non-threading code; required for `<thread>`, `<atomic>`'s wait/notify, semaphores, latches, etc. |
| `-Ifeatures` | hard-coded default              | Lets examples include the shared readable-demo helper as `support/demo.hpp`.    |
| `$EXTRA_COMPILE_FLAGS` | `EXTRA_COMPILE_FLAGS …` arg | File-specific flags (e.g. `-fopenmp`, `-D_GLIBCXX_DEBUG`, `-O3`). |

For a normal runtime example, the output binary lives in
`build/features/<bucket>/<file>` (CMake's standard out-of-source build
layout). Diagnostic, compile-only, and module proof modes instead create their
objects or temporary executable inside that bucket's build directory.
`EXTRA_LIBS` gets added via
`target_link_libraries(... PRIVATE …)` after the source file, e.g. `stdc++exp`
for `std::stacktrace` or `tbb` for parallel STL.

## Per-test arguments you'll see in this repo

| Argument            | Used by which examples                                | What it does |
|---------------------|-------------------------------------------------------|--------------|
| `EXTRA_LIBS stdc++exp` | `std/cpp23/cpp23_print.cpp`, `std/cpp23/cpp23_stacktrace.cpp` | Links libstdc++exp (the *experimental* libstdc++ archive). Required on **GCC 14** for `std::print`/`std::println`/`std::stacktrace`. **GCC 15+** ships them in the main libstdc++ — see [gcc/gcc15/gcc15_default_print.cpp](../features/gcc/gcc15/gcc15_default_print.cpp). |
| `EXTRA_LIBS tbb`    | `std/cpp17/cpp17_parallel_algos.cpp`                  | Links Intel TBB. Parallel STL execution policies (`std::execution::par`, `par_unseq`) are implemented on top of TBB on libstdc++. |
| `EXTRA_COMPILE_FLAGS -fopenmp` | `gccext/openmp/gccext_openmp_parallel_for.cpp` | Enables OpenMP pragmas. The function automatically forwards `-fopenmp` to the link step too so libgomp is linked. |
| `EXTRA_COMPILE_FLAGS -fcontracts` | `std/cpp26/cpp26_contracts_basic.cpp` | Enables GCC 16's C++26 contracts implementation. The example supplies the program-replaceable violation handler and runs satisfied preconditions, postconditions, and `contract_assert`. |
| `EXTRA_COMPILE_FLAGS -freflection` | `std/cpp26/cpp26_reflection_basic.cpp` | Enables C++26 static-reflection front-end (GCC 16+). |
| `EXTRA_COMPILE_FLAGS -D_GLIBCXX_DEBUG` | `gccext/sanitize/integration/gccext_glibcxx_debug.cpp` | Turns on libstdc++ debug-mode containers + iterator checks. |
| `EXTRA_COMPILE_FLAGS -O3 -fopt-info-vec-optimized` | `gccext/codegen/gccext_autovectorize.cpp` | Bumps optimisation and requires a successful compiler report containing `loop vectorized`. Skipped in sanitizer modes because instrumentation changes this code-generation decision. |
| `GCC_EXTRA_COMPILE_FLAGS -Wno-error=maybe-uninitialized` | `std/cpp11/cpp11_regex.cpp` | Keeps a GCC 15 libstdc++ `<regex>` false positive visible under sanitizer instrumentation without weakening other warnings. `GCC_EXTRA_COMPILE_FLAGS` because clang has no `-Wmaybe-uninitialized` and would reject the flag. |
| `EXTRA_COMPILE_FLAGS -mfma` | `gcc/defaults/gccdef_fp_contract.cpp` | Enables x86 FMA codegen so the default `-ffp-contract=fast` can fuse `a*b + c`. `ARCH x86`; the `_aarch64` twin needs no flag because FMA is baseline there. |
| `GCC_EXTRA_COMPILE_FLAGS -Wno-experimental-fmv-target` | `gccext/attributes/gccext_target_clones_aarch64.cpp` | Silences GCC 15's experimental-FMV warning for AArch64 `target_clones` under `-Werror` (GCC 14 ignores the unknown `-Wno-` option). GCC-only: clang rejects the unknown option and its AArch64 FMV needs no suppression. |

## How to read a `gcc_feature_test()` call

A typical entry in `features/std/cpp23/CMakeLists.txt`:

```cmake
gcc_feature_test(cpp23_stacktrace  STD c++23  MIN_GCC 14  TOPIC stl
                 EXTRA_LIBS stdc++exp  MIN_LIBSTDCXX 14)
```

| Argument            | Required? | Meaning |
|---------------------|-----------|---------|
| `<name>`            | yes       | Matches `<name>.cpp` in the same folder. |
| `STD <std>`         | yes       | Passed to `-std=`. One of `c++11`, `c++14`, `c++17`, `c++20`, `c++23`, `c++26` — or `default`, which omits `-std` so the example sees the compiler's default dialect. |
| `MIN_GCC <n>`       | yes       | Tests on older GCC are not registered in that configure mode. |
| `TOPIC <name>`      | yes       | Becomes the CTest label — filter with `ctest -L <topic>`. |
| `MAX_GCC <n>`       | no        | Symmetric upper bound; useful for since-removed behaviour. |
| `MIN_CLANG <n>`     | no        | First clang major that supports the example; older clang lanes skip it. `MIN_GCC`/`MAX_GCC` are ignored under clang. |
| `GCC_ONLY`          | no        | Never registered in the clang lane: a GCC extension, a GCC-release premise, or a feature no mainline clang implements yet. Mutually exclusive with `MIN_CLANG`. |
| `MIN_LIBSTDCXX <n>` | no        | Require `_GLIBCXX_RELEASE >= n`. Independent of `MIN_GCC`. |
| `MAX_LIBSTDCXX <n>` | no        | Symmetric upper bound. |
| `EXTRA_LIBS <lib…>` | no        | Linker libraries (without `-l`). Added via `target_link_libraries`. |
| `EXTRA_COMPILE_FLAGS <flag…>` | no | Per-test compile flags (e.g. `-fopenmp`, `-D_GLIBCXX_DEBUG`). |
| `GCC_EXTRA_COMPILE_FLAGS <flag…>` | no | Per-test compile flags applied only under GCC — for flags clang does not know. |
| `REQUIRES_SANITIZER <san…>` | no | Test enabled only when `-DGCC_FEATURE_SANITIZE=` includes one of the listed sanitizers. |
| `SKIP_SANITIZER <san…>` | no | Test disabled when active sanitizer matches (e.g. TSan + OpenMP is unsupported). |
| `REQUIRES_ANALYZER` | no        | Test only enabled with `-DGCC_FEATURE_ANALYZER=ON`. CTest compiles it at `-O0 -fanalyzer`; requires `EXPECT_COMPILE_OUTPUT`. |
| `WILL_FAIL`         | no        | Runtime test is expected to fail; requires `EXPECT_OUTPUT` so unrelated failures do not pass. |
| `EXPECT_OUTPUT <re>` | required with `WILL_FAIL` | Output regex that must match the expected sanitizer report text. |
| `EXPECT_RUN_OUTPUT <re>` | no | Successful binary output must match the regex. |
| `EXPECT_COMPILE_OUTPUT <re>` | no | A successful compiler invocation must emit a matching diagnostic/report. Used for analyzer and vectorizer claims. |
| `COMPILE_ONLY` | no | The source must compile cleanly but is not linked or run. Use only when the current runtime does not yet export an implemented header API; explain that with `SKIP_REASON`. |
| `MODULE_INTERFACE <file>` | no | Compiles the interface first, then the importer, links both objects, and runs the result. |
| `EXPERIMENTAL`      | no        | Feature not yet supported by current GCC; combined with `EXPECT_ERROR`, the expected outcome is a compile failure. |
| `EXPECT_ERROR <re>` | no (required by `EXPERIMENTAL`) | The test compiles only, must FAIL, and the regex must match the diagnostic. Also used standalone by the `gcc-diagnostics` demos, which pair it with `-Werror=<warning>` to assert a specific warning fires. |
| `EXPECT_ERROR_GCC <re>` / `EXPECT_ERROR_CLANG <re>` | no | Split form of `EXPECT_ERROR` for when the two compilers word the same diagnostic differently; always given together, each lane asserts its own regex. |
| `ALLOW_WARNINGS` | no | Removes baseline `-Werror`. Intended only when a warning is the behavior being asserted. |
| `TAGS <tag…>` | no | Adds cross-cutting CTest labels in addition to `TOPIC`; `essential` is the curated 24-example path. |
| `STATUS <status>` | no | Coverage result such as `covered`, `negative`, or `compile-only`. Usually inferred from the proof mode. |
| `ARCH <arch>` | no | Target-architecture constraint (`portable`, `x86`, or `aarch64`); on a non-matching compiler target the example is skipped at configure time. Recorded in `coverage.yml`; defaults to `portable`. |
| `FEATURE_MACRO <macro>` | no | Relevant SD-6 capability macro recorded in coverage metadata. |
| `PROPOSAL <paper>` | no | WG21 paper identifier for proposal-level traceability. |
| `PREREQUISITES <name…>` | no | Earlier examples that introduce the concepts this one builds on. |
| `SKIP_REASON <text>` | no | Rationale for partial or compile-only coverage. |

The libstdc++ release is detected at configure time by asking `g++` to
preprocess a tiny translation unit that includes `<version>` and reports
`_GLIBCXX_RELEASE`. The detected value is printed by `cmake -S . -B build`
and cached in `LIBSTDCXX_RELEASE`. If the macro is unavailable (very old
toolchain), `MIN_LIBSTDCXX` gates are not enforced.

## Reproducing a build by hand

```bash
# Build everything (or one target) inside the container. Working tree is
# read-only; the build directory lives in a named container volume.
./scripts/container-dev.sh 15                            # verbose CTest by default
./scripts/container-dev.sh 15 -- -R cpp23_stacktrace     # one test, still verbose
./scripts/container-dev.sh 15 -- -j                      # parallel CTest when speed matters
./scripts/container-dev.sh clang22                       # clang-22 cross-check lane
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
   The clang lane has the same pairing:
   [../containers/clang.Containerfile](../containers/clang.Containerfile)
   mirrors the `clang-22` job's install step in ci.yml.
