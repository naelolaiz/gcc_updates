# GCC extensions: sanitizer integration (clean demos)

_Folder: `features/gccext/sanitize/integration/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-sanitize](#gcc-sanitize)

## gcc-sanitize

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_assume_under_sanitize.cpp](gccext_assume_under_sanitize.cpp) | c++23 | 14 | [[assume(expr)]] is a hint to the optimizer; under -fsanitize=undefined GCC additionally checks the assumption at runtime. |
| [gccext_glibcxx_debug.cpp](gccext_glibcxx_debug.cpp) | c++17 | 13 | -D_GLIBCXX_DEBUG turns on libstdc++'s debug mode -- iterator misuse, out-of-range access, mismatched containers all assert at runtime. |
| [gccext_no_sanitize_attribute.cpp](gccext_no_sanitize_attribute.cpp) | c++17 | 13 | __attribute__((no_sanitize("undefined"))) opts a single function out of UBSan -- useful when a hot path provably can't trip the check. |
