# Release smoke tests prove what the gcc:15 toolchain ships; GCC_ONLY by premise.
gcc_feature_test(gcc15_default_print  STD c++23  MIN_GCC 15  TOPIC gcc-release
                 GCC_ONLY)

# New-in-GCC-15 diagnostic: compile-only, must fail with the expected text.
gcc_feature_test(gcc15_warn_deprecated_literal_operator  STD c++23  MIN_GCC 15  TOPIC gcc-diagnostics
                 GCC_ONLY
                 EXTRA_COMPILE_FLAGS -Werror=deprecated-literal-operator
                 EXPECT_ERROR "space between quotes and suffix is deprecated")
