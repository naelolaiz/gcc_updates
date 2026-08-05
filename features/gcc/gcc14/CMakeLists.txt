# print/println are in libstdc++exp on GCC 14 (main libstdc++ from 15 on);
# linking stdc++exp stays harmless on newer GCCs.
gcc_feature_test(gcc14_libstdcxx_print_exp  STD c++23  MIN_GCC 14  TOPIC gcc-release
                 EXTRA_LIBS stdc++exp)
gcc_feature_test(gcc14_libstdcxx_ranges_to  STD c++23  MIN_GCC 14  TOPIC gcc-release)
