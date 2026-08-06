# print/println are in libstdc++exp on GCC 14 (main libstdc++ from 15 on);
# linking stdc++exp stays harmless on newer GCCs.
gcc_feature_test(gcc14_libstdcxx_print_exp  STD c++23  MIN_GCC 14  TOPIC gcc-release
                 EXTRA_LIBS stdc++exp)
gcc_feature_test(gcc14_libstdcxx_ranges_to  STD c++23  MIN_GCC 14  TOPIC gcc-release)
gcc_feature_test(gcc14_hardened_bundle       STD c++17  MIN_GCC 14  TOPIC gcc-release
                 TAGS hardening compiler-flags
                 EXTRA_COMPILE_FLAGS -fhardened)

# New-in-GCC-14 diagnostic: compile-only, must fail with the expected text.
gcc_feature_test(gcc14_warn_calloc_transposed  STD c++17  MIN_GCC 14  TOPIC gcc-diagnostics
                 EXTRA_COMPILE_FLAGS -Werror=calloc-transposed-args
                 EXPECT_ERROR "sizes specified with .sizeof. in the earlier argument")
