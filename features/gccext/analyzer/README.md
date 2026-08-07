# GCC extensions: -fanalyzer compile-time demos

_Folder: `features/gccext/analyzer/`. 4 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [diagnostics](#diagnostics)
- [essential](#essential)
- [gcc-analyzer](#gcc-analyzer)
- [memory-safety](#memory-safety)

## diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_analyzer_double_free.cpp](gccext_analyzer_double_free.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer flags a double-free that only fires on a control-flow branch the runtime might never hit; runtime sanitizers can miss this if the test inputs don't reach that path. |
| [gccext_analyzer_leak.cpp](gccext_analyzer_leak.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows an early-return path that loses the last pointer to an allocation and reports a leak without executing the program. |
| [gccext_analyzer_null_deref.cpp](gccext_analyzer_null_deref.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss. |
| [gccext_analyzer_use_after_free.cpp](gccext_analyzer_use_after_free.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer warns at compile time about use-after-free on a path through main(), without requiring that path to run. |

## essential

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_analyzer_null_deref.cpp](gccext_analyzer_null_deref.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss. |

## gcc-analyzer

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_analyzer_double_free.cpp](gccext_analyzer_double_free.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer flags a double-free that only fires on a control-flow branch the runtime might never hit; runtime sanitizers can miss this if the test inputs don't reach that path. |
| [gccext_analyzer_leak.cpp](gccext_analyzer_leak.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows an early-return path that loses the last pointer to an allocation and reports a leak without executing the program. |
| [gccext_analyzer_null_deref.cpp](gccext_analyzer_null_deref.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss. |
| [gccext_analyzer_use_after_free.cpp](gccext_analyzer_use_after_free.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer warns at compile time about use-after-free on a path through main(), without requiring that path to run. |

## memory-safety

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_analyzer_double_free.cpp](gccext_analyzer_double_free.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer flags a double-free that only fires on a control-flow branch the runtime might never hit; runtime sanitizers can miss this if the test inputs don't reach that path. |
| [gccext_analyzer_leak.cpp](gccext_analyzer_leak.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows an early-return path that loses the last pointer to an allocation and reports a leak without executing the program. |
| [gccext_analyzer_null_deref.cpp](gccext_analyzer_null_deref.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer follows the path where a function may return nullptr and is then dereferenced unconditionally; demonstrates path-sensitivity that lint checkers miss. |
| [gccext_analyzer_use_after_free.cpp](gccext_analyzer_use_after_free.cpp) | c++17 | GCC >= 16 | covered | -fanalyzer warns at compile time about use-after-free on a path through main(), without requiring that path to run. |
