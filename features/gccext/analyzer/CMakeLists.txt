# All four need GCC 16+ for the analyzer to recognise these patterns,
# and they're compile-only (the binaries deliberately contain UB).
gcc_feature_test(gccext_analyzer_double_free      STD c++17  MIN_GCC 16  TOPIC gcc-analyzer
                 TAGS diagnostics memory-safety
                 REQUIRES_ANALYZER ALLOW_WARNINGS
                 EXPECT_COMPILE_OUTPUT "Wanalyzer-double-free")
gcc_feature_test(gccext_analyzer_leak             STD c++17  MIN_GCC 16  TOPIC gcc-analyzer
                 TAGS diagnostics memory-safety
                 REQUIRES_ANALYZER ALLOW_WARNINGS
                 EXPECT_COMPILE_OUTPUT "Wanalyzer-malloc-leak")
gcc_feature_test(gccext_analyzer_null_deref       STD c++17  MIN_GCC 16  TOPIC gcc-analyzer
                 TAGS essential diagnostics memory-safety
                 REQUIRES_ANALYZER ALLOW_WARNINGS
                 EXPECT_COMPILE_OUTPUT "Wanalyzer-null-dereference")
gcc_feature_test(gccext_analyzer_use_after_free   STD c++17  MIN_GCC 16  TOPIC gcc-analyzer
                 TAGS diagnostics memory-safety
                 REQUIRES_ANALYZER ALLOW_WARNINGS
                 EXPECT_COMPILE_OUTPUT "Wanalyzer-use-after-free")
