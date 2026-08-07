# GCC 15 release-notes examples

_Folder: `features/gcc/gcc15/`. 2 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [gcc-diagnostics](#gcc-diagnostics)
- [gcc-release](#gcc-release)

## gcc-diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc15_warn_deprecated_literal_operator.cpp](gcc15_warn_deprecated_literal_operator.cpp) | c++23 | GCC >= 15 | negative | GCC 15 introduced -Wdeprecated-literal-operator: C++23 deprecated the space between "" and the suffix when declaring a literal operator. Built with -Werror=deprecated-literal-operator; CTest asserts the diagnostic fires. |

## gcc-release

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc15_default_print.cpp](gcc15_default_print.cpp) | c++23 | GCC >= 15 | covered | GCC 15 promoted std::print/std::println from libstdc++exp into the main libstdc++ -- no extra link flags needed. |
