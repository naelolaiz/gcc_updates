gcc_feature_test(cpp20_abbreviated_templates  STD c++20  MIN_GCC 13  TOPIC templates)
gcc_feature_test(cpp20_atomic_flag_test       STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_atomic_ref             STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_atomic_shared_ptr      STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_atomic_wait            STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_barrier                STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_binary_semaphore       STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_bind_front             STD c++20  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp20_bit_ops                STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_chrono_calendar        STD c++20  MIN_GCC 14  TOPIC chrono)
gcc_feature_test(cpp20_cmp_utilities          STD c++20  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp20_concepts_intro         STD c++20  MIN_GCC 14  TOPIC concepts
                 TAGS essential templates diagnostics
                 PREREQUISITES cpp17_if_constexpr)
gcc_feature_test(cpp20_concepts_constraint_error
                 STD c++20  MIN_GCC 14  TOPIC concepts
                 TAGS diagnostics templates  STATUS negative
                 PREREQUISITES cpp20_concepts_intro
                 EXPECT_ERROR "constraints not satisfied")
gcc_feature_test(cpp20_concepts_requires_expr STD c++20  MIN_GCC 14  TOPIC concepts)
gcc_feature_test(cpp20_consteval              STD c++20  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp20_constinit              STD c++20  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp20_coroutine_generator    STD c++20  MIN_GCC 14  TOPIC coroutines)
gcc_feature_test(cpp20_counting_semaphore     STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_ctad_aggregates        STD c++20  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp20_ctad_alias             STD c++20  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp20_designated_init        STD c++20  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp20_designated_init_order_error
                 STD c++20  MIN_GCC 14  TOPIC language
                 TAGS diagnostics initialization  STATUS negative
                 PREREQUISITES cpp20_designated_init
                 EXPECT_ERROR_GCC "designator order"
                 EXPECT_ERROR_CLANG "designators to be specified in declaration order")
gcc_feature_test(cpp20_endian                 STD c++20  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp20_erase_if               STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_explicit_bool          STD c++20  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp20_format                 STD c++20  MIN_GCC 14  TOPIC format
                 TAGS essential io)
gcc_feature_test(cpp20_is_constant_evaluated  STD c++20  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp20_jthread                STD c++20  MIN_GCC 14  TOPIC threading
                 TAGS essential cancellation
                 PREREQUISITES cpp11_thread)
gcc_feature_test(cpp20_lambdas                STD c++20  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp20_latch                  STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_lerp_midpoint          STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_map_contains           STD c++20  MIN_GCC 14  TOPIC stl)
# GCC_ONLY: the module fixture drives GCC's single-pass -fmodules flow;
# clang needs a separate --precompile step producing a .pcm.
gcc_feature_test(cpp20_modules_basic           STD c++20  MIN_GCC 16  TOPIC language
                 TAGS essential modules build-system  STATUS covered
                 PROPOSAL P1103R3  FEATURE_MACRO __cpp_modules
                 MODULE_INTERFACE cpp20_modules_math.cpp
                 GCC_ONLY
                 EXTRA_COMPILE_FLAGS -fmodules)
gcc_feature_test(cpp20_nttp_class             STD c++20  MIN_GCC 14  TOPIC templates)
gcc_feature_test(cpp20_numbers                STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_ranges_algorithms      STD c++20  MIN_GCC 14  TOPIC ranges)
gcc_feature_test(cpp20_ranges_borrowed_range  STD c++20  MIN_GCC 14  TOPIC ranges
                 TAGS lifetimes iterators
                 PREREQUISITES cpp20_ranges_algorithms)
gcc_feature_test(cpp20_ranges_views           STD c++20  MIN_GCC 14  TOPIC ranges
                 TAGS essential views algorithms)
gcc_feature_test(cpp20_source_location        STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_spaceship              STD c++20  MIN_GCC 14  TOPIC language)
gcc_feature_test(cpp20_span                   STD c++20  MIN_GCC 14  TOPIC stl
                 TAGS essential lifetimes views
                 PREREQUISITES cpp17_string_view)
gcc_feature_test(cpp20_ssize                  STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_stop_token             STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_string_starts_ends     STD c++20  MIN_GCC 14  TOPIC stl)
gcc_feature_test(cpp20_syncstream             STD c++20  MIN_GCC 14  TOPIC threading)
gcc_feature_test(cpp20_to_address             STD c++20  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp20_type_identity          STD c++20  MIN_GCC 13  TOPIC templates)
gcc_feature_test(cpp20_using_enum             STD c++20  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp20_views_keys_values      STD c++20  MIN_GCC 13  TOPIC ranges)
gcc_feature_test(cpp20_views_split            STD c++20  MIN_GCC 13  TOPIC ranges)
