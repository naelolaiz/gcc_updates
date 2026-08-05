# GCC extensions: builtins & vector_size

_Folder: `features/gccext/builtins/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-builtins](#gcc-builtins)

## gcc-builtins

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_builtin_assume_aligned.cpp](gccext_builtin_assume_aligned.cpp) | c++17 | 13 | __builtin_assume_aligned(p, N) tells the optimizer 'p is N-byte aligned' -- enables aligned vector loads. |
| [gccext_builtin_constant_p.cpp](gccext_builtin_constant_p.cpp) | c++17 | 13 | __builtin_constant_p(x) is true at compile time iff x is a constant expression. Used to pick a faster path for known inputs. |
| [gccext_builtin_cpu_supports.cpp](gccext_builtin_cpu_supports.cpp) | c++17 | 13 | __builtin_cpu_supports("feature") tests a CPU capability at runtime (after __builtin_cpu_init) -- the manual dispatch mechanism underneath [[gnu::target_clones]]'s automatic IFUNC dispatch. |
| [gccext_builtin_expect.cpp](gccext_builtin_expect.cpp) | c++17 | 13 | __builtin_expect(expr, val) is the GCC predecessor of [[likely]]/[[unlikely]]; still useful in C and pre-C++20. |
| [gccext_builtin_prefetch.cpp](gccext_builtin_prefetch.cpp) | c++17 | 13 | __builtin_prefetch(addr, rw, locality) asks the CPU to pull a cache line early; purely a hint -- a wrong guess costs memory bandwidth, never correctness. |
| [gccext_int128.cpp](gccext_int128.cpp) | c++17 | 13 | __int128 / unsigned __int128 give 128-bit integer arithmetic on 64-bit targets -- full-width 64x64 multiplies without intrinsics (wrap in __extension__ to stay -Wpedantic-clean). |
| [gccext_vector_extensions.cpp](gccext_vector_extensions.cpp) | c++17 | 13 | __attribute__((vector_size(N))) makes a real SIMD vector type; element-wise +,-,*, etc. compile to one instruction. |
