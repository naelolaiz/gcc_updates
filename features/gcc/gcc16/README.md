# GCC 16 release-notes examples

_Folder: `features/gcc/gcc16/`. 4 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [extensions](#extensions)
- [gcc-diagnostics](#gcc-diagnostics)
- [gcc-release](#gcc-release)
- [type-traits](#type-traits)

## extensions

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc16_int128_type_traits.cpp](gcc16_int128_type_traits.cpp) | c++17 | GCC >= 16 | covered | GCC 16 makes standard type traits recognize __int128 even in strict -std=c++NN modes, aligning strict and GNU dialect behavior. |

## gcc-diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc16_warn_cpp26_compat.cpp](gcc16_warn_cpp26_compat.cpp) | c++23 | GCC >= 16 | negative | GCC 16 introduced -Wc++26-compat: identifiers that become keywords in C++26 (like contract_assert) are flagged when compiling as an older standard. Built as C++23 with -Werror=c++26-compat; CTest asserts the diagnostic fires. |

## gcc-release

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc16_cpp26_features_default.cpp](gcc16_cpp26_features_default.cpp) | c++26 | GCC >= 16 | covered | GCC 16 ships ~two-thirds of C++26 by default and tracks draft revisions live: the saturation API is the renamed saturating_add/saturating_cast (__cpp_lib_saturation_arithmetic 202603), replacing GCC 14/15's add_sat. |
| [gcc16_int128_type_traits.cpp](gcc16_int128_type_traits.cpp) | c++17 | GCC >= 16 | covered | GCC 16 makes standard type traits recognize __int128 even in strict -std=c++NN modes, aligning strict and GNU dialect behavior. |

## type-traits

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gcc16_int128_type_traits.cpp](gcc16_int128_type_traits.cpp) | c++17 | GCC >= 16 | covered | GCC 16 makes standard type traits recognize __int128 even in strict -std=c++NN modes, aligning strict and GNU dialect behavior. |
