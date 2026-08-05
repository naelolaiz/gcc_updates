gcc_feature_test(gcc16_cpp26_features_default  STD c++26  MIN_GCC 16  TOPIC gcc-release)

# New-in-GCC-16 diagnostic: compile-only, must fail with the expected text.
gcc_feature_test(gcc16_warn_cpp26_compat       STD c++23  MIN_GCC 16  TOPIC gcc-diagnostics
                 EXTRA_COMPILE_FLAGS -Werror=c++26-compat
                 EXPECT_ERROR "is a keyword in C\\+\\+26")
