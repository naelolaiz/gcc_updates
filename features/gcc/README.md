# `features/gcc/` — per-release smoke tests

Each release folder exercises features *introduced or stabilised* in that
version, plus one diagnostic that release added. The narrative "here's what changed" lives in
[../../docs/gcc-changelogs.md](../../docs/gcc-changelogs.md); these are the
companion compile-and-run demos.

| Folder | Headline |
|--------|----------|
| [`defaults/`](defaults/) | Cross-release toolchain defaults: default `-std` dialect (changed in GCC 15/16), `-ffp-contract=fast`, PIE-vs-packaging. Facts in [../../docs/default-changes.md](../../docs/default-changes.md). |
| [`gcc13/`](gcc13/) | First GCC with a usable libstdc++ `<format>`; many C++23 ranges views land. |
| [`gcc14/`](gcc14/) | `std::ranges::to`, `std::generator`, and `std::print` arrive (`std::stacktrace` still links `-lstdc++exp`). First C++26 features and `-fhardened` are added. |
| [`gcc15/`](gcc15/) | `std::format` / `std::println` handle ranges and tuples; `flat_map`, `flat_set`, and `import std` arrive. Default C bumps to C23; pack indexing and delete-with-reason land. |
| [`gcc16/`](gcc16/) | C++20 becomes the default dialect and libstdc++'s C++20 support is no longer experimental; C++26 reflection, contracts, expansion statements, and broad library coverage land. |
