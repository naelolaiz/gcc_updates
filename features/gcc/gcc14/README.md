# GCC 14 release-notes examples

_Folder: `features/gcc/gcc14/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [gcc-release](#gcc-release)

## gcc-release

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gcc14_libstdcxx_print_exp.cpp](gcc14_libstdcxx_print_exp.cpp) | c++23 | 14 | GCC 14 first shipped std::print/std::println in libstdc++, still in the experimental library -- linking -lstdc++exp is required (GCC 15 later moved it into the main library; see gcc15_default_print). |
| [gcc14_libstdcxx_ranges_to.cpp](gcc14_libstdcxx_ranges_to.cpp) | c++23 | 14 | GCC 14 was the first release where libstdc++ shipped std::ranges::to in the main library (no extra flags). |
