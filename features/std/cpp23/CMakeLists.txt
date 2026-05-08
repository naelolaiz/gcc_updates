# features/std/cpp23/CMakeLists.txt
# Each source file has one gcc_feature_test() registration below.
# Run all:         ctest --verbose
# Filter by topic: ctest --verbose -L ranges
# Filter by name:  ctest -R cpp23_flat_map

gcc_feature_test(cpp23_assume                       STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_auto_decay_copy              STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_byteswap                     STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_deducing_this                STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_expected                     STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_flat_map                     STD c++23  MIN_GCC 15  TOPIC stl)
gcc_feature_test(cpp23_flat_set                     STD c++23  MIN_GCC 15  TOPIC stl)
gcc_feature_test(cpp23_generator                    STD c++23  MIN_GCC 14  TOPIC coroutines)
gcc_feature_test(cpp23_if_consteval                 STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_mdspan                       STD c++23  MIN_GCC 16  TOPIC stl)
gcc_feature_test(cpp23_move_only_function           STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_multidim_subscript           STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_optional_monadic             STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_out_ptr                      STD c++23  MIN_GCC 14  TOPIC stl)
# EXTRA_LIBS stdc++exp: pre-GCC-15, std::print/std::println live in the experimental libstdc++exp.
# GCC 15+ promoted them into the main libstdc++ -- see features/gcc/gcc15/CMakeLists.txt.
gcc_feature_test(cpp23_print                        STD c++23  MIN_GCC 14  TOPIC stl
                 EXTRA_LIBS stdc++exp)
gcc_feature_test(cpp23_ranges_adjacent              STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_cartesian_product     STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_chunk_slide           STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_enumerate             STD c++23  MIN_GCC 14  TOPIC ranges)
# MIN_LIBSTDCXX 13: these algorithms live in the libstdc++ runtime; the gate skips
# the example when the host's libstdc++ is older than 13, even if the GCC frontend
# is new enough.
gcc_feature_test(cpp23_ranges_find_last             STD c++23  MIN_GCC 14  TOPIC ranges
                 MIN_LIBSTDCXX 13)
gcc_feature_test(cpp23_ranges_fold                  STD c++23  MIN_GCC 14  TOPIC ranges
                 MIN_LIBSTDCXX 13)
gcc_feature_test(cpp23_ranges_iota_algorithm        STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_join_with             STD c++23  MIN_GCC 14  TOPIC ranges)
# MIN_LIBSTDCXX 16: these algorithms aren't present in older libstdc++ runtimes;
# the gate skips the example when the host runtime is below release 16.
gcc_feature_test(cpp23_ranges_starts_ends_contains  STD c++23  MIN_GCC 14  TOPIC ranges
                 MIN_LIBSTDCXX 16)
gcc_feature_test(cpp23_ranges_to                    STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_zip                   STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_recursive_lambda             STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_resize_and_overwrite         STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_size_t_literal               STD c++23  MIN_GCC 14  TOPIC language)
# EXTRA_LIBS stdc++exp: std::stacktrace symbols live in libstdc++exp -- unlike
# std::print/println, which were moved into main libstdc++ at GCC 15 (see
# features/gcc/gcc15/CMakeLists.txt). MIN_LIBSTDCXX 14 gates older runtimes that
# don't ship the symbol.
gcc_feature_test(cpp23_stacktrace                   STD c++23  MIN_GCC 14  TOPIC stl
                 EXTRA_LIBS stdc++exp  MIN_LIBSTDCXX 14)
gcc_feature_test(cpp23_static_operator              STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_string_contains              STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_to_underlying                STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_unreachable                  STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_views_as_const_as_rvalue     STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_views_repeat                 STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_views_stride                 STD c++23  MIN_GCC 14  TOPIC ranges)
