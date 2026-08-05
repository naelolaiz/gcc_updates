gcc_feature_test(gcc13_libstdcxx_format          STD c++20  MIN_GCC 13  TOPIC gcc-release)

# New-in-GCC-13 diagnostic: compile-only, must fail with the expected text.
gcc_feature_test(gcc13_warn_dangling_reference   STD c++17  MIN_GCC 13  TOPIC gcc-diagnostics
                 EXTRA_COMPILE_FLAGS -Werror=dangling-reference
                 EXPECT_ERROR "dangling reference to a temporary")
