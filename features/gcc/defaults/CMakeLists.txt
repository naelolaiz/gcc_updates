# Toolchain-default demos. gccdef_dialect uses STD default: it is compiled
# WITHOUT -std on purpose, to observe the compiler's default dialect.
# The whole bucket documents GCC's own toolchain defaults, hence GCC_ONLY.
gcc_feature_test(gccdef_dialect      STD default  MIN_GCC 13  TOPIC gcc-defaults
                 GCC_ONLY)
# Contraction is an optimized-codegen effect; the -O1 + instrumentation of
# sanitizer builds doesn't fuse, so the demo only runs in normal configs.
# ARCH x86: -mfma and the __builtin_cpu_supports("fma") runtime probe are
# x86-specific (on aarch64 FMA is baseline and the builtin family differs).
gcc_feature_test(gccdef_fp_contract  STD c++17    MIN_GCC 13  TOPIC gcc-defaults
                 EXTRA_COMPILE_FLAGS -mfma  ARCH x86  GCC_ONLY
                 SKIP_SANITIZER address undefined thread leak)
# AArch64 twin: FMA is baseline there, so no extra flag and no CPU probe.
gcc_feature_test(gccdef_fp_contract_aarch64  STD c++17  MIN_GCC 13  TOPIC gcc-defaults
                 ARCH aarch64  GCC_ONLY
                 SKIP_SANITIZER address undefined thread leak)
gcc_feature_test(gccdef_pie_default  STD c++17    MIN_GCC 13  TOPIC gcc-defaults
                 GCC_ONLY)
