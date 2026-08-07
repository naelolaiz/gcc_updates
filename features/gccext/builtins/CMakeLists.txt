gcc_feature_test(gccext_builtin_assume_aligned  STD c++17  MIN_GCC 13  TOPIC gcc-builtins)
gcc_feature_test(gccext_builtin_constant_p      STD c++17  MIN_GCC 13  TOPIC gcc-builtins)
# ARCH x86: __builtin_cpu_init/__builtin_cpu_supports are x86 builtins.
gcc_feature_test(gccext_builtin_cpu_supports    STD c++17  MIN_GCC 13  TOPIC gcc-builtins
                 ARCH x86)
gcc_feature_test(gccext_builtin_expect          STD c++17  MIN_GCC 13  TOPIC gcc-builtins)
gcc_feature_test(gccext_builtin_prefetch        STD c++17  MIN_GCC 13  TOPIC gcc-builtins)
gcc_feature_test(gccext_int128                  STD c++17  MIN_GCC 13  TOPIC gcc-builtins)
gcc_feature_test(gccext_vector_extensions       STD c++17  MIN_GCC 13  TOPIC gcc-builtins)
