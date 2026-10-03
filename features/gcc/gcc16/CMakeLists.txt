# Release smoke tests prove what the gcc:16 toolchain ships; GCC_ONLY by premise.
gcc_feature_test(gcc16_cpp26_features_default  STD c++26  MIN_GCC 16  TOPIC gcc-release
                 GCC_ONLY)
gcc_feature_test(gcc16_int128_type_traits       STD c++17  MIN_GCC 16  TOPIC gcc-release
                 TAGS type-traits extensions
                 GCC_ONLY)

# New-in-GCC-16 diagnostic: compile-only, must fail with the expected text.
gcc_feature_test(gcc16_warn_cpp26_compat       STD c++23  MIN_GCC 16  TOPIC gcc-diagnostics
                 GCC_ONLY
                 EXTRA_COMPILE_FLAGS -Werror=c++26-compat
                 EXPECT_ERROR "is a keyword in C\\+\\+26")
