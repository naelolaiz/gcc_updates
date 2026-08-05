# Topic index

_All 18 topics across every bucket, generated from `gcc_feature_test()` metadata; regenerate with `./scripts/podman-dev.sh <ver> readme`. Topics double as CTest labels: `ctest -L threading` runs one topic everywhere._

## Topics

- [chrono](#chrono)
- [concepts](#concepts)
- [coroutines](#coroutines)
- [format](#format)
- [gcc-analyzer](#gcc-analyzer)
- [gcc-attributes](#gcc-attributes)
- [gcc-builtins](#gcc-builtins)
- [gcc-codegen](#gcc-codegen)
- [gcc-defaults](#gcc-defaults)
- [gcc-openmp](#gcc-openmp)
- [gcc-pragmas](#gcc-pragmas)
- [gcc-release](#gcc-release)
- [gcc-sanitize](#gcc-sanitize)
- [language](#language)
- [ranges](#ranges)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)

## chrono

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp20_chrono_calendar.cpp](std/cpp20/cpp20_chrono_calendar.cpp) | std/cpp20 | c++20 | 14 |

## concepts

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp20_concepts_intro.cpp](std/cpp20/cpp20_concepts_intro.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_concepts_requires_expr.cpp](std/cpp20/cpp20_concepts_requires_expr.cpp) | std/cpp20 | c++20 | 14 |

## coroutines

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp20_coroutine_generator.cpp](std/cpp20/cpp20_coroutine_generator.cpp) | std/cpp20 | c++20 | 14 |
| [cpp23_generator.cpp](std/cpp23/cpp23_generator.cpp) | std/cpp23 | c++23 | 14 |

## format

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp20_format.cpp](std/cpp20/cpp20_format.cpp) | std/cpp20 | c++20 | 14 |

## gcc-analyzer

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_analyzer_double_free.cpp](gccext/analyzer/gccext_analyzer_double_free.cpp) | gccext/analyzer | c++17 | 16 |
| [gccext_analyzer_null_deref.cpp](gccext/analyzer/gccext_analyzer_null_deref.cpp) | gccext/analyzer | c++17 | 16 |
| [gccext_analyzer_use_after_free.cpp](gccext/analyzer/gccext_analyzer_use_after_free.cpp) | gccext/analyzer | c++17 | 16 |

## gcc-attributes

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_attribute_cleanup.cpp](gccext/attributes/gccext_attribute_cleanup.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_attribute_constructor.cpp](gccext/attributes/gccext_attribute_constructor.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_attribute_flatten.cpp](gccext/attributes/gccext_attribute_flatten.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_attribute_hot_cold.cpp](gccext/attributes/gccext_attribute_hot_cold.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_attribute_packed.cpp](gccext/attributes/gccext_attribute_packed.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_attribute_pure_const.cpp](gccext/attributes/gccext_attribute_pure_const.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_attribute_target.cpp](gccext/attributes/gccext_attribute_target.cpp) | gccext/attributes | c++17 | 13 |
| [gccext_likely_unlikely.cpp](gccext/attributes/gccext_likely_unlikely.cpp) | gccext/attributes | c++20 | 13 |
| [gccext_target_clones.cpp](gccext/attributes/gccext_target_clones.cpp) | gccext/attributes | c++17 | 13 |

## gcc-builtins

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_builtin_assume_aligned.cpp](gccext/builtins/gccext_builtin_assume_aligned.cpp) | gccext/builtins | c++17 | 13 |
| [gccext_builtin_constant_p.cpp](gccext/builtins/gccext_builtin_constant_p.cpp) | gccext/builtins | c++17 | 13 |
| [gccext_builtin_cpu_supports.cpp](gccext/builtins/gccext_builtin_cpu_supports.cpp) | gccext/builtins | c++17 | 13 |
| [gccext_builtin_expect.cpp](gccext/builtins/gccext_builtin_expect.cpp) | gccext/builtins | c++17 | 13 |
| [gccext_builtin_prefetch.cpp](gccext/builtins/gccext_builtin_prefetch.cpp) | gccext/builtins | c++17 | 13 |
| [gccext_int128.cpp](gccext/builtins/gccext_int128.cpp) | gccext/builtins | c++17 | 13 |
| [gccext_vector_extensions.cpp](gccext/builtins/gccext_vector_extensions.cpp) | gccext/builtins | c++17 | 13 |

## gcc-codegen

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_autovectorize.cpp](gccext/codegen/gccext_autovectorize.cpp) | gccext/codegen | c++17 | 13 |
| [gccext_inline_asm.cpp](gccext/codegen/gccext_inline_asm.cpp) | gccext/codegen | c++17 | 13 |

## gcc-defaults

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccdef_dialect.cpp](gcc/defaults/gccdef_dialect.cpp) | gcc/defaults | default | 13 |
| [gccdef_fp_contract.cpp](gcc/defaults/gccdef_fp_contract.cpp) | gcc/defaults | c++17 | 13 |
| [gccdef_pie_default.cpp](gcc/defaults/gccdef_pie_default.cpp) | gcc/defaults | c++17 | 13 |

## gcc-openmp

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_openmp_parallel_for.cpp](gccext/openmp/gccext_openmp_parallel_for.cpp) | gccext/openmp | c++17 | 13 |

## gcc-pragmas

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_diagnostic_pragma.cpp](gccext/pragmas/gccext_diagnostic_pragma.cpp) | gccext/pragmas | c++17 | 13 |

## gcc-release

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gcc13_libstdcxx_format.cpp](gcc/gcc13/gcc13_libstdcxx_format.cpp) | gcc/gcc13 | c++20 | 13 |
| [gcc14_libstdcxx_print_exp.cpp](gcc/gcc14/gcc14_libstdcxx_print_exp.cpp) | gcc/gcc14 | c++23 | 14 |
| [gcc14_libstdcxx_ranges_to.cpp](gcc/gcc14/gcc14_libstdcxx_ranges_to.cpp) | gcc/gcc14 | c++23 | 14 |
| [gcc15_default_print.cpp](gcc/gcc15/gcc15_default_print.cpp) | gcc/gcc15 | c++23 | 15 |
| [gcc16_cpp26_features_default.cpp](gcc/gcc16/gcc16_cpp26_features_default.cpp) | gcc/gcc16 | c++26 | 16 |

## gcc-sanitize

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [gccext_asan_double_free.cpp](gccext/sanitize/asan/gccext_asan_double_free.cpp) | gccext/sanitize/asan | c++17 | 13 |
| [gccext_asan_heap_buffer_overflow.cpp](gccext/sanitize/asan/gccext_asan_heap_buffer_overflow.cpp) | gccext/sanitize/asan | c++17 | 13 |
| [gccext_asan_heap_use_after_free.cpp](gccext/sanitize/asan/gccext_asan_heap_use_after_free.cpp) | gccext/sanitize/asan | c++17 | 13 |
| [gccext_asan_stack_buffer_overflow.cpp](gccext/sanitize/asan/gccext_asan_stack_buffer_overflow.cpp) | gccext/sanitize/asan | c++17 | 13 |
| [gccext_assume_under_sanitize.cpp](gccext/sanitize/integration/gccext_assume_under_sanitize.cpp) | gccext/sanitize/integration | c++23 | 14 |
| [gccext_glibcxx_debug.cpp](gccext/sanitize/integration/gccext_glibcxx_debug.cpp) | gccext/sanitize/integration | c++17 | 13 |
| [gccext_no_sanitize_attribute.cpp](gccext/sanitize/integration/gccext_no_sanitize_attribute.cpp) | gccext/sanitize/integration | c++17 | 13 |
| [gccext_lsan_simple_leak.cpp](gccext/sanitize/leak/gccext_lsan_simple_leak.cpp) | gccext/sanitize/leak | c++17 | 13 |
| [gccext_tsan_data_race.cpp](gccext/sanitize/tsan/gccext_tsan_data_race.cpp) | gccext/sanitize/tsan | c++17 | 13 |
| [gccext_ubsan_null_deref.cpp](gccext/sanitize/ubsan/gccext_ubsan_null_deref.cpp) | gccext/sanitize/ubsan | c++17 | 13 |
| [gccext_ubsan_shift.cpp](gccext/sanitize/ubsan/gccext_ubsan_shift.cpp) | gccext/sanitize/ubsan | c++17 | 13 |
| [gccext_ubsan_signed_overflow.cpp](gccext/sanitize/ubsan/gccext_ubsan_signed_overflow.cpp) | gccext/sanitize/ubsan | c++17 | 13 |

## language

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp11_alignof_alignas.cpp](std/cpp11/cpp11_alignof_alignas.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_attribute_noreturn.cpp](std/cpp11/cpp11_attribute_noreturn.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_auto.cpp](std/cpp11/cpp11_auto.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_constexpr.cpp](std/cpp11/cpp11_constexpr.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_delete_default.cpp](std/cpp11/cpp11_delete_default.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_explicit_conversion.cpp](std/cpp11/cpp11_explicit_conversion.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_initializer_list.cpp](std/cpp11/cpp11_initializer_list.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_inline_namespace.cpp](std/cpp11/cpp11_inline_namespace.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_lambda.cpp](std/cpp11/cpp11_lambda.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_noexcept.cpp](std/cpp11/cpp11_noexcept.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_nullptr.cpp](std/cpp11/cpp11_nullptr.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_override_final.cpp](std/cpp11/cpp11_override_final.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_range_for.cpp](std/cpp11/cpp11_range_for.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_raw_string.cpp](std/cpp11/cpp11_raw_string.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_rvalue_move.cpp](std/cpp11/cpp11_rvalue_move.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_scoped_enum.cpp](std/cpp11/cpp11_scoped_enum.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_static_assert.cpp](std/cpp11/cpp11_static_assert.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_trailing_return.cpp](std/cpp11/cpp11_trailing_return.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_uniform_init.cpp](std/cpp11/cpp11_uniform_init.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_user_defined_literal.cpp](std/cpp11/cpp11_user_defined_literal.cpp) | std/cpp11 | c++11 | 13 |
| [cpp14_binary_literals.cpp](std/cpp14/cpp14_binary_literals.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_generic_lambda.cpp](std/cpp14/cpp14_generic_lambda.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_lambda_init_capture.cpp](std/cpp14/cpp14_lambda_init_capture.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_relaxed_constexpr.cpp](std/cpp14/cpp14_relaxed_constexpr.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_return_type_deduction.cpp](std/cpp14/cpp14_return_type_deduction.cpp) | std/cpp14 | c++14 | 13 |
| [cpp17_attributes.cpp](std/cpp17/cpp17_attributes.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_constexpr_lambda.cpp](std/cpp17/cpp17_constexpr_lambda.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_init_statement.cpp](std/cpp17/cpp17_init_statement.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_inline_variables.cpp](std/cpp17/cpp17_inline_variables.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_nested_namespace.cpp](std/cpp17/cpp17_nested_namespace.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_structured_bindings.cpp](std/cpp17/cpp17_structured_bindings.cpp) | std/cpp17 | c++17 | 13 |
| [cpp20_consteval.cpp](std/cpp20/cpp20_consteval.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_constinit.cpp](std/cpp20/cpp20_constinit.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_designated_init.cpp](std/cpp20/cpp20_designated_init.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_is_constant_evaluated.cpp](std/cpp20/cpp20_is_constant_evaluated.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_lambdas.cpp](std/cpp20/cpp20_lambdas.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_spaceship.cpp](std/cpp20/cpp20_spaceship.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_using_enum.cpp](std/cpp20/cpp20_using_enum.cpp) | std/cpp20 | c++20 | 13 |
| [cpp23_assume.cpp](std/cpp23/cpp23_assume.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_auto_decay_copy.cpp](std/cpp23/cpp23_auto_decay_copy.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_if_consteval.cpp](std/cpp23/cpp23_if_consteval.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_size_t_literal.cpp](std/cpp23/cpp23_size_t_literal.cpp) | std/cpp23 | c++23 | 14 |
| [cpp26_contracts_basic.cpp](std/cpp26/cpp26_contracts_basic.cpp) | std/cpp26 | c++26 | 15 |
| [cpp26_delete_reason.cpp](std/cpp26/cpp26_delete_reason.cpp) | std/cpp26 | c++26 | 15 |
| [cpp26_pack_indexing.cpp](std/cpp26/cpp26_pack_indexing.cpp) | std/cpp26 | c++26 | 15 |
| [cpp26_static_assert_messages.cpp](std/cpp26/cpp26_static_assert_messages.cpp) | std/cpp26 | c++26 | 15 |

## ranges

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp20_ranges_algorithms.cpp](std/cpp20/cpp20_ranges_algorithms.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_ranges_views.cpp](std/cpp20/cpp20_ranges_views.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_views_keys_values.cpp](std/cpp20/cpp20_views_keys_values.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_views_split.cpp](std/cpp20/cpp20_views_split.cpp) | std/cpp20 | c++20 | 13 |
| [cpp23_ranges_adjacent.cpp](std/cpp23/cpp23_ranges_adjacent.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_cartesian_product.cpp](std/cpp23/cpp23_ranges_cartesian_product.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_chunk_slide.cpp](std/cpp23/cpp23_ranges_chunk_slide.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_enumerate.cpp](std/cpp23/cpp23_ranges_enumerate.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_find_last.cpp](std/cpp23/cpp23_ranges_find_last.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_fold.cpp](std/cpp23/cpp23_ranges_fold.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_iota_algorithm.cpp](std/cpp23/cpp23_ranges_iota_algorithm.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_join_with.cpp](std/cpp23/cpp23_ranges_join_with.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_starts_ends_contains.cpp](std/cpp23/cpp23_ranges_starts_ends_contains.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_to.cpp](std/cpp23/cpp23_ranges_to.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_ranges_zip.cpp](std/cpp23/cpp23_ranges_zip.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_views_as_const_as_rvalue.cpp](std/cpp23/cpp23_views_as_const_as_rvalue.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_views_chunk_by.cpp](std/cpp23/cpp23_views_chunk_by.cpp) | std/cpp23 | c++23 | 13 |
| [cpp23_views_repeat.cpp](std/cpp23/cpp23_views_repeat.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_views_stride.cpp](std/cpp23/cpp23_views_stride.cpp) | std/cpp23 | c++23 | 14 |

## stl

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp11_algorithms_new.cpp](std/cpp11/cpp11_algorithms_new.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_array.cpp](std/cpp11/cpp11_array.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_bind.cpp](std/cpp11/cpp11_bind.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_chrono.cpp](std/cpp11/cpp11_chrono.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_emplace.cpp](std/cpp11/cpp11_emplace.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_exception_ptr.cpp](std/cpp11/cpp11_exception_ptr.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_forward_list.cpp](std/cpp11/cpp11_forward_list.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_function.cpp](std/cpp11/cpp11_function.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_hash.cpp](std/cpp11/cpp11_hash.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_iota.cpp](std/cpp11/cpp11_iota.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_iterator_helpers.cpp](std/cpp11/cpp11_iterator_helpers.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_random.cpp](std/cpp11/cpp11_random.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_reference_wrapper.cpp](std/cpp11/cpp11_reference_wrapper.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_regex.cpp](std/cpp11/cpp11_regex.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_smart_ptrs.cpp](std/cpp11/cpp11_smart_ptrs.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_system_error.cpp](std/cpp11/cpp11_system_error.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_tuple.cpp](std/cpp11/cpp11_tuple.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_unordered_map.cpp](std/cpp11/cpp11_unordered_map.cpp) | std/cpp11 | c++11 | 13 |
| [cpp14_cbegin_cend.cpp](std/cpp14/cpp14_cbegin_cend.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_dual_range_algorithms.cpp](std/cpp14/cpp14_dual_range_algorithms.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_exchange.cpp](std/cpp14/cpp14_exchange.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_heterogeneous_lookup.cpp](std/cpp14/cpp14_heterogeneous_lookup.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_make_unique.cpp](std/cpp14/cpp14_make_unique.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_quoted.cpp](std/cpp14/cpp14_quoted.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_std_literals.cpp](std/cpp14/cpp14_std_literals.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_tuple_by_type.cpp](std/cpp14/cpp14_tuple_by_type.cpp) | std/cpp14 | c++14 | 13 |
| [cpp17_any.cpp](std/cpp17/cpp17_any.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_byte.cpp](std/cpp17/cpp17_byte.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_charconv.cpp](std/cpp17/cpp17_charconv.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_clamp.cpp](std/cpp17/cpp17_clamp.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_filesystem.cpp](std/cpp17/cpp17_filesystem.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_gcd_lcm_sample.cpp](std/cpp17/cpp17_gcd_lcm_sample.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_invoke_apply.cpp](std/cpp17/cpp17_invoke_apply.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_launder.cpp](std/cpp17/cpp17_launder.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_memory_resource.cpp](std/cpp17/cpp17_memory_resource.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_node_handles.cpp](std/cpp17/cpp17_node_handles.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_not_fn_as_const.cpp](std/cpp17/cpp17_not_fn_as_const.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_optional.cpp](std/cpp17/cpp17_optional.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_parallel_algos.cpp](std/cpp17/cpp17_parallel_algos.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_searchers.cpp](std/cpp17/cpp17_searchers.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_shared_ptr_array.cpp](std/cpp17/cpp17_shared_ptr_array.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_string_view.cpp](std/cpp17/cpp17_string_view.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_variant.cpp](std/cpp17/cpp17_variant.cpp) | std/cpp17 | c++17 | 13 |
| [cpp20_bind_front.cpp](std/cpp20/cpp20_bind_front.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_bit_ops.cpp](std/cpp20/cpp20_bit_ops.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_cmp_utilities.cpp](std/cpp20/cpp20_cmp_utilities.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_endian.cpp](std/cpp20/cpp20_endian.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_erase_if.cpp](std/cpp20/cpp20_erase_if.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_lerp_midpoint.cpp](std/cpp20/cpp20_lerp_midpoint.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_map_contains.cpp](std/cpp20/cpp20_map_contains.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_numbers.cpp](std/cpp20/cpp20_numbers.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_source_location.cpp](std/cpp20/cpp20_source_location.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_span.cpp](std/cpp20/cpp20_span.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_ssize.cpp](std/cpp20/cpp20_ssize.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_string_starts_ends.cpp](std/cpp20/cpp20_string_starts_ends.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_to_address.cpp](std/cpp20/cpp20_to_address.cpp) | std/cpp20 | c++20 | 13 |
| [cpp23_byteswap.cpp](std/cpp23/cpp23_byteswap.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_expected.cpp](std/cpp23/cpp23_expected.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_flat_map.cpp](std/cpp23/cpp23_flat_map.cpp) | std/cpp23 | c++23 | 15 |
| [cpp23_flat_set.cpp](std/cpp23/cpp23_flat_set.cpp) | std/cpp23 | c++23 | 15 |
| [cpp23_format_ranges.cpp](std/cpp23/cpp23_format_ranges.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_forward_like.cpp](std/cpp23/cpp23_forward_like.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_invoke_r.cpp](std/cpp23/cpp23_invoke_r.cpp) | std/cpp23 | c++23 | 13 |
| [cpp23_mdspan.cpp](std/cpp23/cpp23_mdspan.cpp) | std/cpp23 | c++23 | 16 |
| [cpp23_move_only_function.cpp](std/cpp23/cpp23_move_only_function.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_optional_monadic.cpp](std/cpp23/cpp23_optional_monadic.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_out_ptr.cpp](std/cpp23/cpp23_out_ptr.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_print.cpp](std/cpp23/cpp23_print.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_resize_and_overwrite.cpp](std/cpp23/cpp23_resize_and_overwrite.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_spanstream.cpp](std/cpp23/cpp23_spanstream.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_stacktrace.cpp](std/cpp23/cpp23_stacktrace.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_start_lifetime_as.cpp](std/cpp23/cpp23_start_lifetime_as.cpp) | std/cpp23 | c++23 | 16 |
| [cpp23_stdfloat.cpp](std/cpp23/cpp23_stdfloat.cpp) | std/cpp23 | c++23 | 13 |
| [cpp23_string_contains.cpp](std/cpp23/cpp23_string_contains.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_to_underlying.cpp](std/cpp23/cpp23_to_underlying.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_unreachable.cpp](std/cpp23/cpp23_unreachable.cpp) | std/cpp23 | c++23 | 14 |
| [cpp26_saturation_arith.cpp](std/cpp26/cpp26_saturation_arith.cpp) | std/cpp26 | c++26 | 14 |
| [cpp26_span_at.cpp](std/cpp26/cpp26_span_at.cpp) | std/cpp26 | c++26 | 15 |
| [cpp26_text_encoding.cpp](std/cpp26/cpp26_text_encoding.cpp) | std/cpp26 | c++26 | 15 |

## templates

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp11_alias_template.cpp](std/cpp11/cpp11_alias_template.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_ratio.cpp](std/cpp11/cpp11_ratio.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_type_traits.cpp](std/cpp11/cpp11_type_traits.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_variadic_templates.cpp](std/cpp11/cpp11_variadic_templates.cpp) | std/cpp11 | c++11 | 13 |
| [cpp14_integer_sequence.cpp](std/cpp14/cpp14_integer_sequence.cpp) | std/cpp14 | c++14 | 13 |
| [cpp14_variable_templates.cpp](std/cpp14/cpp14_variable_templates.cpp) | std/cpp14 | c++14 | 13 |
| [cpp17_ctad_basic.cpp](std/cpp17/cpp17_ctad_basic.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_fold_expressions.cpp](std/cpp17/cpp17_fold_expressions.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_if_constexpr.cpp](std/cpp17/cpp17_if_constexpr.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_type_trait_helpers.cpp](std/cpp17/cpp17_type_trait_helpers.cpp) | std/cpp17 | c++17 | 13 |
| [cpp20_abbreviated_templates.cpp](std/cpp20/cpp20_abbreviated_templates.cpp) | std/cpp20 | c++20 | 13 |
| [cpp20_ctad_aggregates.cpp](std/cpp20/cpp20_ctad_aggregates.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_ctad_alias.cpp](std/cpp20/cpp20_ctad_alias.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_explicit_bool.cpp](std/cpp20/cpp20_explicit_bool.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_nttp_class.cpp](std/cpp20/cpp20_nttp_class.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_type_identity.cpp](std/cpp20/cpp20_type_identity.cpp) | std/cpp20 | c++20 | 13 |
| [cpp23_deducing_this.cpp](std/cpp23/cpp23_deducing_this.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_multidim_subscript.cpp](std/cpp23/cpp23_multidim_subscript.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_recursive_lambda.cpp](std/cpp23/cpp23_recursive_lambda.cpp) | std/cpp23 | c++23 | 14 |
| [cpp23_static_operator.cpp](std/cpp23/cpp23_static_operator.cpp) | std/cpp23 | c++23 | 14 |
| [cpp26_reflection_basic.cpp](std/cpp26/cpp26_reflection_basic.cpp) | std/cpp26 | c++26 | 16 |

## threading

| Example | Bucket | std | min-gcc |
| ------- | ------ | --- | ------- |
| [cpp11_async_future.cpp](std/cpp11/cpp11_async_future.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_atomic.cpp](std/cpp11/cpp11_atomic.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_condition_variable.cpp](std/cpp11/cpp11_condition_variable.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_mutex_lock.cpp](std/cpp11/cpp11_mutex_lock.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_thread.cpp](std/cpp11/cpp11_thread.cpp) | std/cpp11 | c++11 | 13 |
| [cpp11_thread_local.cpp](std/cpp11/cpp11_thread_local.cpp) | std/cpp11 | c++11 | 13 |
| [cpp14_shared_timed_mutex.cpp](std/cpp14/cpp14_shared_timed_mutex.cpp) | std/cpp14 | c++14 | 13 |
| [cpp17_scoped_lock.cpp](std/cpp17/cpp17_scoped_lock.cpp) | std/cpp17 | c++17 | 13 |
| [cpp17_shared_mutex.cpp](std/cpp17/cpp17_shared_mutex.cpp) | std/cpp17 | c++17 | 13 |
| [cpp20_atomic_flag_test.cpp](std/cpp20/cpp20_atomic_flag_test.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_atomic_ref.cpp](std/cpp20/cpp20_atomic_ref.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_atomic_shared_ptr.cpp](std/cpp20/cpp20_atomic_shared_ptr.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_atomic_wait.cpp](std/cpp20/cpp20_atomic_wait.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_barrier.cpp](std/cpp20/cpp20_barrier.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_binary_semaphore.cpp](std/cpp20/cpp20_binary_semaphore.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_counting_semaphore.cpp](std/cpp20/cpp20_counting_semaphore.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_jthread.cpp](std/cpp20/cpp20_jthread.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_latch.cpp](std/cpp20/cpp20_latch.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_stop_token.cpp](std/cpp20/cpp20_stop_token.cpp) | std/cpp20 | c++20 | 14 |
| [cpp20_syncstream.cpp](std/cpp20/cpp20_syncstream.cpp) | std/cpp20 | c++20 | 14 |
