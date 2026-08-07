# `features/` — example tree

Every `.cpp` here is a single-file program. Build metadata lives in the
folder's `CMakeLists.txt` as one `gcc_feature_test()` call per source file.
The tree is split three ways:

| Subfolder | Contents | Typical reader |
|-----------|----------|----------------|
| [`std/`](std/) | C++ standard-library and language features, grouped by standard. | "What's in C++NN?" |
| [`gcc/`](gcc/) | One smoke-test per GCC release (13, 14, 15, 16) tied to a notable change in that version. | "What did GCC NN actually ship?" |
| [`gccext/`](gccext/) | GCC extensions that aren't in any C++ standard: attributes, builtins, OpenMP, vector_size types, target multi-versioning, diagnostic pragmas, inline asm. | "How do I use this GCC-specific feature?" |

Each leaf folder has its own `README.md` index listing examples grouped by
topic, generated from the `gcc_feature_test()` metadata and each file's
`// description:` line (`./scripts/container-dev.sh <ver> readme` regenerates;
every configure checks them for drift). [TOPICS.md](TOPICS.md) is the
cross-bucket view: every topic label with its examples from all buckets,
generated and drift-checked the same way. The narrative version of "what each release shipped" lives in
[../docs/gcc-changelogs.md](../docs/gcc-changelogs.md). The flag reference is
in [../docs/compiler-flags.md](../docs/compiler-flags.md).

Top-level reading path is in the [main README](../README.md#suggested-reference-path).
