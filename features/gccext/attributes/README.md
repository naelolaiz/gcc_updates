# GCC extensions: attributes

_Folder: `features/gccext/attributes/`. 8 example(s) across 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-attributes

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_attribute_cleanup.cpp](gccext_attribute_cleanup.cpp) | c++17 | 13 | __attribute__((cleanup(fn))) calls fn(&var) when var goes out of scope -- C-style RAII; redundant in C++ but works there too. |
| [gccext_attribute_flatten.cpp](gccext_attribute_flatten.cpp) | c++17 | 13 | [[gnu::flatten]] inlines every direct call inside the marked function -- careful: bloats code. |
| [gccext_attribute_hot_cold.cpp](gccext_attribute_hot_cold.cpp) | c++17 | 13 | [[gnu::hot]] / [[gnu::cold]] tell the optimizer how often a function runs; influences inlining and code layout. |
| [gccext_attribute_packed.cpp](gccext_attribute_packed.cpp) | c++17 | 13 | __attribute__((packed)) removes padding between struct members; useful for binary protocols, but unaligned access can be slower. |
| [gccext_attribute_pure_const.cpp](gccext_attribute_pure_const.cpp) | c++17 | 13 | [[gnu::pure]] = no side effects (may read globals); [[gnu::const]] = depends only on args. Both let the compiler eliminate redundant calls. |
| [gccext_attribute_target.cpp](gccext_attribute_target.cpp) | c++17 | 13 | [[gnu::target("avx2")]] compiles ONE function with extra ISA flags; the rest of the TU stays at the default ISA. |
| [gccext_likely_unlikely.cpp](gccext_likely_unlikely.cpp) | c++20 | 13 | [[likely]] and [[unlikely]] (C++20 standard) hint branch frequency to the optimizer; GCC implements them via __builtin_expect. |
| [gccext_target_clones.cpp](gccext_target_clones.cpp) | c++17 | 13 | [[gnu::target_clones("default,avx2,avx512f")]] generates multiple versions; an IFUNC dispatches to the best at startup. |
