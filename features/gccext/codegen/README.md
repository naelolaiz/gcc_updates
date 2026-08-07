# GCC extensions: codegen (auto-vectorisation, inline asm)

_Folder: `features/gccext/codegen/`. 5 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [architecture](#architecture)
- [gcc-codegen](#gcc-codegen)
- [inline-assembly](#inline-assembly)
- [performance](#performance)
- [simd](#simd)

## architecture

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_inline_asm.cpp](gccext_inline_asm.cpp) | c++17 | GCC >= 13 | covered | Basic GCC extended inline asm. Demonstrates the input/output operand syntax. x86_64-only; tagged experimental for portability. |

## gcc-codegen

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_autovectorize.cpp](gccext_autovectorize.cpp) | c++17 | GCC >= 13 | covered | A SAXPY loop is checked twice: runtime assertions prove its result, and GCC's optimization report proves that -O3 vectorized it. |
| [gccext_inline_asm.cpp](gccext_inline_asm.cpp) | c++17 | GCC >= 13 | covered | Basic GCC extended inline asm. Demonstrates the input/output operand syntax. x86_64-only; tagged experimental for portability. |

## inline-assembly

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_inline_asm.cpp](gccext_inline_asm.cpp) | c++17 | GCC >= 13 | covered | Basic GCC extended inline asm. Demonstrates the input/output operand syntax. x86_64-only; tagged experimental for portability. |

## performance

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_autovectorize.cpp](gccext_autovectorize.cpp) | c++17 | GCC >= 13 | covered | A SAXPY loop is checked twice: runtime assertions prove its result, and GCC's optimization report proves that -O3 vectorized it. |

## simd

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_autovectorize.cpp](gccext_autovectorize.cpp) | c++17 | GCC >= 13 | covered | A SAXPY loop is checked twice: runtime assertions prove its result, and GCC's optimization report proves that -O3 vectorized it. |
