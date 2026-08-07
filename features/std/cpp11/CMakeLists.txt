gcc_feature_test(cpp11_algorithms_new       STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_alias_template       STD c++11  MIN_GCC 13  TOPIC templates)
gcc_feature_test(cpp11_alignof_alignas      STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_array                STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_async_future         STD c++11  MIN_GCC 13  TOPIC threading)
gcc_feature_test(cpp11_atomic               STD c++11  MIN_GCC 13  TOPIC threading)
gcc_feature_test(cpp11_attribute_noreturn   STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_auto                 STD c++11  MIN_GCC 13  TOPIC language
                 TAGS essential type-deduction)
gcc_feature_test(cpp11_bind                 STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_chrono               STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_condition_variable   STD c++11  MIN_GCC 13  TOPIC threading)
gcc_feature_test(cpp11_constexpr            STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_delete_default       STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_emplace              STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_exception_ptr        STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_explicit_conversion  STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_forward_list         STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_function             STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_hash                 STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_initializer_list     STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_inline_namespace     STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_iota                 STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_iterator_helpers     STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_lambda               STD c++11  MIN_GCC 13  TOPIC language
                 TAGS essential callables)
gcc_feature_test(cpp11_mutex_lock           STD c++11  MIN_GCC 13  TOPIC threading)
gcc_feature_test(cpp11_noexcept             STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_nullptr              STD c++11  MIN_GCC 13  TOPIC language
                 TAGS essential type-safety)
gcc_feature_test(cpp11_override_final       STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_random               STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_range_for            STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_ratio                STD c++11  MIN_GCC 13  TOPIC templates)
gcc_feature_test(cpp11_raw_string           STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_reference_wrapper    STD c++11  MIN_GCC 13  TOPIC stl)
# -Wmaybe-uninitialized is a GCC-only warning (and a GCC-only false positive
# in <regex>), so the workaround flag must not reach clang.
gcc_feature_test(cpp11_regex                STD c++11  MIN_GCC 13  TOPIC stl
                 GCC_EXTRA_COMPILE_FLAGS -Wno-error=maybe-uninitialized)
gcc_feature_test(cpp11_rvalue_move          STD c++11  MIN_GCC 13  TOPIC language
                 TAGS essential ownership value-categories)
gcc_feature_test(cpp11_scoped_enum          STD c++11  MIN_GCC 13  TOPIC language
                 TAGS essential type-safety)
gcc_feature_test(cpp11_smart_ptrs           STD c++11  MIN_GCC 13  TOPIC stl
                 TAGS essential ownership memory
                 PREREQUISITES cpp11_rvalue_move)
gcc_feature_test(cpp11_static_assert        STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_system_error         STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_thread               STD c++11  MIN_GCC 13  TOPIC threading)
gcc_feature_test(cpp11_thread_local         STD c++11  MIN_GCC 13  TOPIC threading)
gcc_feature_test(cpp11_trailing_return      STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_tuple                STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_type_traits          STD c++11  MIN_GCC 13  TOPIC templates)
gcc_feature_test(cpp11_uniform_init         STD c++11  MIN_GCC 13  TOPIC language
                 TAGS essential initialization)
gcc_feature_test(cpp11_uniform_init_narrowing_error
                 STD c++11  MIN_GCC 13  TOPIC language
                 TAGS diagnostics initialization  STATUS negative
                 PREREQUISITES cpp11_uniform_init
                 EXPECT_ERROR_GCC "narrowing conversion"
                 EXPECT_ERROR_CLANG "cannot be narrowed to")
gcc_feature_test(cpp11_unordered_map        STD c++11  MIN_GCC 13  TOPIC stl)
gcc_feature_test(cpp11_user_defined_literal STD c++11  MIN_GCC 13  TOPIC language)
gcc_feature_test(cpp11_variadic_templates   STD c++11  MIN_GCC 13  TOPIC templates)
