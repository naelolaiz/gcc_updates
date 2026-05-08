# All three need GCC 16+ for the analyzer to recognise these patterns,
# and they're compile-only (the binaries deliberately contain UB).
gcc_feature_test(gccext_analyzer_double_free      STD c++17  MIN_GCC 16  TOPIC gcc-analyzer  REQUIRES_ANALYZER)
gcc_feature_test(gccext_analyzer_null_deref       STD c++17  MIN_GCC 16  TOPIC gcc-analyzer  REQUIRES_ANALYZER)
gcc_feature_test(gccext_analyzer_use_after_free   STD c++17  MIN_GCC 16  TOPIC gcc-analyzer  REQUIRES_ANALYZER)
