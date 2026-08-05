# GCC extensions: builtins & vector_size

_Folder: `features/gccext/builtins/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-builtins](#gcc-builtins)

## gcc-builtins

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_builtin_assume_aligned.cpp](gccext_builtin_assume_aligned.cpp) | c++17 | 13 | __builtin_assume_aligned(p, N) tells the optimizer 'p is N-byte aligned' -- enables aligned vector loads. |
| [gccext_builtin_constant_p.cpp](gccext_builtin_constant_p.cpp) | c++17 | 13 | __builtin_constant_p(x) is true at compile time iff x is a constant expression. Used to pick a faster path for known inputs. |
| [gccext_builtin_expect.cpp](gccext_builtin_expect.cpp) | c++17 | 13 | __builtin_expect(expr, val) is the GCC predecessor of [[likely]]/[[unlikely]]; still useful in C and pre-C++20. |
| [gccext_vector_extensions.cpp](gccext_vector_extensions.cpp) | c++17 | 13 | __attribute__((vector_size(N))) makes a real SIMD vector type; element-wise +,-,*, etc. compile to one instruction. |
