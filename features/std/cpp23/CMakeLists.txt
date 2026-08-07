# features/std/cpp23/CMakeLists.txt
# Each source file has one gcc_feature_test() registration below.
# Run all:         ctest --verbose
# Filter by topic: ctest --verbose -L ranges
# Filter by name:  ctest -R cpp23_flat_map

gcc_feature_test(cpp23_assume                       STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_allocate_at_least            STD c++23  MIN_GCC 16  TOPIC stl
                 MIN_LIBSTDCXX 16  TAGS memory allocators
                 FEATURE_MACRO __cpp_lib_allocate_at_least)
gcc_feature_test(cpp23_auto_decay_copy              STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_byteswap                     STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_deducing_this                STD c++23  MIN_GCC 14  TOPIC templates
                 TAGS essential callables value-categories
                 PREREQUISITES cpp11_rvalue_move)
gcc_feature_test(cpp23_expected                     STD c++23  MIN_GCC 14  TOPIC stl
                 TAGS essential error-handling value-types
                 PREREQUISITES cpp17_optional)
gcc_feature_test(cpp23_flat_map                     STD c++23  MIN_GCC 15  TOPIC stl)
gcc_feature_test(cpp23_flat_set                     STD c++23  MIN_GCC 15  TOPIC stl)
gcc_feature_test(cpp23_format_ranges                STD c++23  MIN_GCC 14  TOPIC stl
                 MIN_LIBSTDCXX 15)
gcc_feature_test(cpp23_forward_like                 STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_generator                    STD c++23  MIN_GCC 14  TOPIC coroutines)
gcc_feature_test(cpp23_if_consteval                 STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_invoke_r                     STD c++23  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp23_mdspan                       STD c++23  MIN_GCC 16  TOPIC stl)
gcc_feature_test(cpp23_move_only_function           STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_multidim_subscript           STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_optional_monadic             STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_out_ptr                      STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_print                        STD c++23  MIN_GCC 14  TOPIC stl
                 TAGS essential io format
                 EXTRA_LIBS stdc++exp
                 EXPECT_RUN_OUTPUT "hello from std::println")
gcc_feature_test(cpp23_ranges_adjacent              STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_cartesian_product     STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_chunk_slide           STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_enumerate             STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_find_last             STD c++23  MIN_GCC 14  TOPIC ranges
                 MIN_LIBSTDCXX 13)
gcc_feature_test(cpp23_ranges_fold                  STD c++23  MIN_GCC 14  TOPIC ranges
                 MIN_LIBSTDCXX 13)
gcc_feature_test(cpp23_ranges_iota_algorithm        STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_join_with             STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_ranges_shift                 STD c++23  MIN_GCC 16  TOPIC ranges
                 MIN_LIBSTDCXX 16  TAGS algorithms containers)
gcc_feature_test(cpp23_ranges_starts_ends_contains  STD c++23  MIN_GCC 14  TOPIC ranges
                 MIN_LIBSTDCXX 16)
gcc_feature_test(cpp23_ranges_to                    STD c++23  MIN_GCC 14  TOPIC ranges
                 TAGS essential containers
                 PREREQUISITES cpp20_ranges_views)
gcc_feature_test(cpp23_ranges_zip                   STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_recursive_lambda             STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_resize_and_overwrite         STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_size_t_literal               STD c++23  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp23_spanstream                   STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_stacktrace                   STD c++23  MIN_GCC 14  TOPIC stl
                 EXTRA_LIBS stdc++exp  MIN_LIBSTDCXX 14)
gcc_feature_test(cpp23_start_lifetime_as            STD c++23  MIN_GCC 16  TOPIC stl
                 MIN_LIBSTDCXX 16)
gcc_feature_test(cpp23_static_operator              STD c++23  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp23_stdfloat                     STD c++23  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp23_string_contains              STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_to_underlying                STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_unreachable                  STD c++23  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp23_views_as_const_as_rvalue     STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_views_chunk_by               STD c++23  MIN_GCC 13  TOPIC ranges)
gcc_feature_test(cpp23_views_repeat                 STD c++23  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp23_views_stride                 STD c++23  MIN_GCC 14  TOPIC ranges)
