# GCC extensions: codegen (auto-vectorisation, inline asm)

_Folder: `features/gccext/codegen/`. 2 example(s) across 1 topic(s). Auto-generated from each file's `// gcc-test:` header — do not edit by hand. Run `python3 scripts/discover.py --emit-docs` to refresh._

## gcc-codegen

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_autovectorize.cpp](gccext_autovectorize.cpp) | c++17 | 13 | A loop the auto-vectorizer should turn into SIMD at -O3. We can't easily ASSERT vectorization at runtime; use 'g++ -O3 -fopt-info-vec' (or this build) to confirm. |
| [gccext_inline_asm.cpp](gccext_inline_asm.cpp) | c++17 | 13 | Basic GCC extended inline asm. Demonstrates the input/output operand syntax. x86_64-only; tagged experimental for portability. |
