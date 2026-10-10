# GCC 14 release-notes examples

_Folder: `features/gcc/gcc14/`. 4 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [compiler-flags](#compiler-flags)
- [gcc-diagnostics](#gcc-diagnostics)
- [gcc-release](#gcc-release)
- [hardening](#hardening)

## compiler-flags

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc14_hardened_bundle.cpp](gcc14_hardened_bundle.cpp) | c++17 | GCC >= 14 (GCC only) | covered | GCC 14 introduced -fhardened as an umbrella for production hardening flags; this test proves that stack protection and fortified libc calls are enabled. |

## gcc-diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc14_warn_calloc_transposed.cpp](gcc14_warn_calloc_transposed.cpp) | c++17 | GCC >= 14 (GCC only) | negative | GCC 14 introduced -Wcalloc-transposed-args: calloc(sizeof(T), n) has the arguments backwards (element count first, element size second). Built with -Werror=calloc-transposed-args; CTest asserts the diagnostic fires. |

## gcc-release

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc14_hardened_bundle.cpp](gcc14_hardened_bundle.cpp) | c++17 | GCC >= 14 (GCC only) | covered | GCC 14 introduced -fhardened as an umbrella for production hardening flags; this test proves that stack protection and fortified libc calls are enabled. |
| [gcc14_libstdcxx_print.cpp](gcc14_libstdcxx_print.cpp) | c++23 | GCC >= 14 (GCC only) | covered | GCC 14 first shipped std::print/std::println in libstdc++; on Linux and other POSIX targets they link with default flags (only Windows needs -lstdc++exp, for Unicode console output). |
| [gcc14_libstdcxx_ranges_to.cpp](gcc14_libstdcxx_ranges_to.cpp) | c++23 | GCC >= 14 (GCC only) | covered | GCC 14 was the first release where libstdc++ shipped std::ranges::to in the main library (no extra flags). |

## hardening

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc14_hardened_bundle.cpp](gcc14_hardened_bundle.cpp) | c++17 | GCC >= 14 (GCC only) | covered | GCC 14 introduced -fhardened as an umbrella for production hardening flags; this test proves that stack protection and fortified libc calls are enabled. |
