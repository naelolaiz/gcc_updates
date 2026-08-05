# GCC extensions: codegen (auto-vectorisation, inline asm)

_Folder: `features/gccext/codegen/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-codegen](#gcc-codegen)

## gcc-codegen

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_autovectorize.cpp](gccext_autovectorize.cpp) | c++17 | 13 | A loop the auto-vectorizer should turn into SIMD at -O3. We can't easily ASSERT vectorization at runtime; use 'g++ -O3 -fopt-info-vec' (or this build) to confirm. |
| [gccext_inline_asm.cpp](gccext_inline_asm.cpp) | c++17 | 13 | Basic GCC extended inline asm. Demonstrates the input/output operand syntax. x86_64-only; tagged experimental for portability. |
