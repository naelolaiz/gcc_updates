# GCC 15 release-notes examples

_Folder: `features/gcc/gcc15/`. 2 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [gcc-diagnostics](#gcc-diagnostics)
- [gcc-release](#gcc-release)

## gcc-diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc15_warn_deprecated_literal_operator.cpp](gcc15_warn_deprecated_literal_operator.cpp) | c++23 | GCC >= 15 (GCC only) | negative | GCC 15 introduced -Wdeprecated-literal-operator: C++23 deprecated the space between "" and the suffix when declaring a literal operator. Built with -Werror=deprecated-literal-operator; CTest asserts the diagnostic fires. |

## gcc-release

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc15_libstdcxx_print_ranges.cpp](gcc15_libstdcxx_print_ranges.cpp) | c++23 | GCC >= 15 (GCC only) | covered | GCC 15's libstdc++ formats ranges and tuples (P2286R8, P2585R1), so std::println("{}", v) prints [1, 2, 3] directly -- GCC 14 rejected it because std::vector had no std::formatter. |
