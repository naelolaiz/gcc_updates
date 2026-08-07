gcc_feature_test(cpp26_contracts_basic         STD c++26  MIN_GCC 16  TOPIC language
                 TAGS contracts diagnostics
                 PROPOSAL P2900R14  FEATURE_MACRO __cpp_contracts
                 EXTRA_COMPILE_FLAGS -fcontracts
                 PREREQUISITES cpp11_static_assert)

gcc_feature_test(cpp26_constexpr_exceptions    STD c++26  MIN_GCC 16  TOPIC language
                 TAGS compile-time error-handling
                 PROPOSAL P3068R5  PREREQUISITES cpp11_exception_ptr cpp14_relaxed_constexpr)

gcc_feature_test(cpp26_debugging               STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS diagnostics tooling
                 PROPOSAL P2546R5  FEATURE_MACRO __cpp_lib_debugging
                 COMPILE_ONLY
                 SKIP_REASON "GCC 16 headers declare the API, but this runtime snapshot does not yet export is_debugger_present")

gcc_feature_test(cpp26_delete_reason           STD c++26  MIN_GCC 15  TOPIC language
                 TAGS diagnostics  PROPOSAL P2573R2)
gcc_feature_test(cpp26_delete_reason_diagnostic
                 STD c++26  MIN_GCC 15  TOPIC language
                 TAGS diagnostics  STATUS negative
                 PROPOSAL P2573R2  PREREQUISITES cpp26_delete_reason
                 EXPECT_ERROR "use parse_config.*length is tracked")

gcc_feature_test(cpp26_expansion_statements    STD c++26  MIN_GCC 16  TOPIC language
                 TAGS templates compile-time
                 PROPOSAL P1306R5  FEATURE_MACRO __cpp_expansion_statements
                 PREREQUISITES cpp11_variadic_templates)

gcc_feature_test(cpp26_function_wrappers       STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS callables ownership
                 PROPOSAL P2548R6  FEATURE_MACRO __cpp_lib_copyable_function
                 PREREQUISITES cpp11_function cpp23_move_only_function)

gcc_feature_test(cpp26_indirect_polymorphic    STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS ownership memory value-types
                 PROPOSAL P3019R14  FEATURE_MACRO __cpp_lib_indirect
                 PREREQUISITES cpp11_smart_ptrs)

gcc_feature_test(cpp26_inplace_vector          STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS containers memory
                 PROPOSAL P0843R14  FEATURE_MACRO __cpp_lib_inplace_vector
                 PREREQUISITES cpp11_array)

gcc_feature_test(cpp26_optional_ref            STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS error-handling lifetimes value-types
                 PROPOSAL P2988R12  FEATURE_MACRO __cpp_lib_optional
                 PREREQUISITES cpp17_optional)

gcc_feature_test(cpp26_pack_indexing           STD c++26  MIN_GCC 15  TOPIC language
                 TAGS templates compile-time  PROPOSAL P2662R3
                 FEATURE_MACRO __cpp_pack_indexing  PREREQUISITES cpp11_variadic_templates)

gcc_feature_test(cpp26_reflection_basic        STD c++26  MIN_GCC 16  TOPIC templates
                 TAGS reflection compile-time  PROPOSAL P2996R13
                 FEATURE_MACRO __cpp_impl_reflection
                 EXTRA_COMPILE_FLAGS -freflection)

gcc_feature_test(cpp26_saturation_arith        STD c++26  MIN_GCC 14  TOPIC stl)

# ARCH x86: libstdc++ 16 does not implement <simd> for aarch64 yet ('simd'
# in namespace 'std' does not name a type); see coverage/gaps.yml.
gcc_feature_test(cpp26_simd                    STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS numerics performance simd  ARCH x86
                 PROPOSAL P1928R15  PREREQUISITES gccext_vector_extensions)

gcc_feature_test(cpp26_span_at                 STD c++26  MIN_GCC 15  TOPIC stl)

gcc_feature_test(cpp26_submdspan               STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS containers numerics views
                 PROPOSAL P2630R4  FEATURE_MACRO __cpp_lib_submdspan
                 PREREQUISITES cpp23_mdspan)

gcc_feature_test(cpp26_static_assert_messages  STD c++26  MIN_GCC 15  TOPIC language)

gcc_feature_test(cpp26_structured_binding_pack STD c++26  MIN_GCC 16  TOPIC language
                 TAGS templates compile-time
                 PROPOSAL P1061R10  FEATURE_MACRO __cpp_structured_bindings
                 PREREQUISITES cpp17_structured_bindings cpp11_variadic_templates)

gcc_feature_test(cpp26_uninitialized_diagnostic STD c++26  MIN_GCC 16  TOPIC language
                 TAGS diagnostics safety  STATUS negative
                 PROPOSAL P2795R5
                 EXTRA_COMPILE_FLAGS -Werror=uninitialized
                 EXPECT_ERROR "used uninitialized")

gcc_feature_test(cpp26_philox_engine           STD c++26  MIN_GCC 16  MIN_LIBSTDCXX 16
                 TOPIC stl  TAGS numerics random
                 PROPOSAL P2075R6  FEATURE_MACRO __cpp_lib_philox_engine
                 PREREQUISITES cpp11_random)

# text_encoding::environment() is still in libstdc++exp on GCC 15.
gcc_feature_test(cpp26_text_encoding           STD c++26  MIN_GCC 15  TOPIC stl
                 EXTRA_LIBS stdc++exp)
