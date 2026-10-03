# GCC 13 release-notes examples

_Folder: `features/gcc/gcc13/`. 2 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [gcc-diagnostics](#gcc-diagnostics)
- [gcc-release](#gcc-release)

## gcc-diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc13_warn_dangling_reference.cpp](gcc13_warn_dangling_reference.cpp) | c++17 | GCC >= 13 (GCC only) | negative | GCC 13 introduced -Wdangling-reference: binding a reference to a call that derives its result from a temporary argument is flagged at compile time. Built with -Werror=dangling-reference; CTest asserts the diagnostic fires. |

## gcc-release

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc13_libstdcxx_format.cpp](gcc13_libstdcxx_format.cpp) | c++20 | GCC >= 13 (GCC only) | covered | GCC 13 was the first release where libstdc++ shipped a usable <format> implementation by default. |
