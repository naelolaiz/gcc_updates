# `features/gcc/` — per-release smoke tests

One file per GCC release that exercises a feature *introduced or stabilised*
in that version. The narrative "here's what changed" lives in
[../../docs/gcc-changelogs.md](../../docs/gcc-changelogs.md); these are the
companion compile-and-run demos.

| Folder | Headline |
|--------|----------|
| [`defaults/`](defaults/) | Cross-release toolchain defaults: default `-std` dialect (changed in GCC 15/16), `-ffp-contract=fast`, PIE-vs-packaging. Facts in [../../docs/default-changes.md](../../docs/default-changes.md). |
| [`gcc13/`](gcc13/) | First GCC with a usable libstdc++ `<format>`; many C++23 ranges views land. |
| [`gcc14/`](gcc14/) | `std::ranges::to`, `std::generator`, `std::mdspan` enter main libstdc++. `std::print` / `std::stacktrace` ship in libstdc++exp. C++26 mode added. |
| [`gcc15/`](gcc15/) | `std::print` / `std::println` move into main libstdc++ (no more `-lstdc++exp`). Default C bumped to C23. ~⅔ of C++26 implemented. |
| [`gcc16/`](gcc16/) | More C++26 maturation (reflection, contracts), Intel AVX10, Algol 68 frontend. |
