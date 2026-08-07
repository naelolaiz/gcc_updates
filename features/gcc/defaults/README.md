# GCC toolchain-default behaviour examples

_Folder: `features/gcc/defaults/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [gcc-defaults](#gcc-defaults)

## gcc-defaults

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccdef_dialect.cpp](gccdef_dialect.cpp) | default | GCC >= 13 (GCC only) | covered | Compiled WITHOUT any -std flag to test the compiler's default dialect: gnu++17 on GCC 13-15, gnu++20 from GCC 16 -- code that relies on "whatever the compiler defaults to" changes meaning across releases. |
| [gccdef_fp_contract.cpp](gccdef_fp_contract.cpp) | c++17 | GCC >= 13 (GCC only) | covered | -ffp-contract=fast is GCC's default in every release (13-16) and every dialect, even strict -std=c++NN: a*b+c may compile to one fused multiply-add, so results change when -march gains FMA -- not because you asked for fast-math. |
| [gccdef_fp_contract_aarch64.cpp](gccdef_fp_contract_aarch64.cpp) | c++17 | GCC >= 13 (GCC only) | covered | On AArch64 the default -ffp-contract=fast needs no extra flags at all: FMA is in the base ISA, so a*b + c compiles to one fused fmadd at -O2 and skips the intermediate rounding -- same default, no -mfma, no CPU probe. |
| [gccdef_pie_default.cpp](gccdef_pie_default.cpp) | c++17 | GCC >= 13 (GCC only) | covered | Position-independent executables are a TOOLCHAIN default, not a language one: upstream gcc:N builds default to non-PIE while distro packages (like the Debian g++-16 CI uses) enable -fPIE by default -- __PIE__/__PIC__ tell you which world you're in. |
