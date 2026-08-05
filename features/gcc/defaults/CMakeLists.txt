# Toolchain-default demos. gccdef_dialect uses STD default: it is compiled
# WITHOUT -std on purpose, to observe the compiler's default dialect.
gcc_feature_test(gccdef_dialect      STD default  MIN_GCC 13  TOPIC gcc-defaults)
# Contraction is an optimized-codegen effect; the -O1 + instrumentation of
# sanitizer builds doesn't fuse, so the demo only runs in normal configs.
gcc_feature_test(gccdef_fp_contract  STD c++17    MIN_GCC 13  TOPIC gcc-defaults
                 EXTRA_COMPILE_FLAGS -mfma
                 SKIP_SANITIZER address undefined thread leak)
gcc_feature_test(gccdef_pie_default  STD c++17    MIN_GCC 13  TOPIC gcc-defaults)
