# `features/gccext/` — GCC extensions, grouped by topic

These features aren't in any C++ standard but ship with every modern GCC.
Read these alongside the standard buckets — `[[likely]]` is C++20 *and*
implemented via the same machinery as `__builtin_expect`, etc.

| Folder | Contents |
|--------|----------|
| [`attributes/`](attributes/) | `[[gnu::pure]]` / `[[gnu::const]]` / `[[gnu::hot]]` / `[[gnu::cold]]` / `[[gnu::flatten]]`; `__attribute__((packed))` / `((aligned))` / `((cleanup))`; `[[gnu::target("avx2")]]`; `[[gnu::target_clones(...)]]` for runtime CPU dispatch; `[[likely]]` / `[[unlikely]]`. |
| [`builtins/`](builtins/) | `__builtin_expect`, `__builtin_expect_with_probability`, `__builtin_constant_p`, `__builtin_assume_aligned`; `__attribute__((vector_size))` SIMD types. |
| [`codegen/`](codegen/) | Auto-vectorisable SAXPY at `-O3`; basic extended inline asm. |
| [`openmp/`](openmp/) | `#pragma omp parallel for` with reduction (`-fopenmp`). |
| [`pragmas/`](pragmas/) | `#pragma GCC diagnostic push/pop/ignored`. |
| [`sanitize/`](sanitize/) | Interacting with `-fsanitize=…` builds: opt-out attributes, libstdc++ debug mode. |
| [`analyzer/`](analyzer/) | Compile-time bug-finding via `-fanalyzer` (distinct from runtime sanitizers): use-after-`delete`, path-sensitive null-deref, conditional double-`delete`. Requires GCC 16+ for C++. |
