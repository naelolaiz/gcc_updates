gcc_feature_test(gccext_attribute_cleanup     STD c++17  MIN_GCC 13  TOPIC gcc-attributes)
gcc_feature_test(gccext_attribute_constructor STD c++17  MIN_GCC 13  TOPIC gcc-attributes)
gcc_feature_test(gccext_attribute_flatten     STD c++17  MIN_GCC 13  TOPIC gcc-attributes)
gcc_feature_test(gccext_attribute_hot_cold    STD c++17  MIN_GCC 13  TOPIC gcc-attributes)
gcc_feature_test(gccext_attribute_packed      STD c++17  MIN_GCC 13  TOPIC gcc-attributes)
gcc_feature_test(gccext_attribute_pure_const  STD c++17  MIN_GCC 13  TOPIC gcc-attributes)
# ARCH x86: target("avx2") names an x86 ISA extension; aarch64 GCC rejects it.
gcc_feature_test(gccext_attribute_target      STD c++17  MIN_GCC 13  TOPIC gcc-attributes
                 ARCH x86)
gcc_feature_test(gccext_attribute_target_aarch64  STD c++17  MIN_GCC 13
                 TOPIC gcc-attributes  ARCH aarch64)
gcc_feature_test(gccext_likely_unlikely       STD c++20  MIN_GCC 13  TOPIC gcc-attributes)
# ARCH x86: the avx2 clone is x86-only, and aarch64 function multi-versioning
# is still experimental (-Wexperimental-fmv-target) as of GCC 15.
gcc_feature_test(gccext_target_clones         STD c++17  MIN_GCC 13  TOPIC gcc-attributes
                 ARCH x86  SKIP_SANITIZER thread)
# AArch64 FMV landed in GCC 14; -Wno-experimental-fmv-target silences the
# GCC 15+ experimental warning under -Werror (GCC 14 ignores the option).
gcc_feature_test(gccext_target_clones_aarch64 STD c++17  MIN_GCC 14  TOPIC gcc-attributes
                 ARCH aarch64
                 EXTRA_COMPILE_FLAGS -Wno-experimental-fmv-target
                 SKIP_SANITIZER thread)
