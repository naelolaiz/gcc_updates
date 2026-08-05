# How C++ features evolved across standards

The per-bucket indexes answer "what's in C++NN?". This page answers the other
question: "how did one idea grow from C++11 to C++26?" Each section names the
examples (all runnable via `ctest -R <name>`) that form one storyline, in the
order the language acquired them. For a flat per-topic listing across buckets,
see [features/TOPICS.md](../features/TOPICS.md).

## Compile-time programming

The single biggest arc in modern C++: from "constants with functions attached"
to a second, full programming language that runs inside the compiler.

1. [`cpp11_constexpr`](../features/std/cpp11/cpp11_constexpr.cpp) — C++11
   constexpr functions are a single return statement; recursion is the only
   loop.
2. [`cpp14_relaxed_constexpr`](../features/std/cpp14/cpp14_relaxed_constexpr.cpp)
   — loops, branches, and local mutation become legal; real algorithms move
   to compile time.
3. [`cpp17_if_constexpr`](../features/std/cpp17/cpp17_if_constexpr.cpp) +
   [`cpp17_constexpr_lambda`](../features/std/cpp17/cpp17_constexpr_lambda.cpp)
   — compile-time *branching on types* inside one template, and lambdas join
   the party.
4. [`cpp20_consteval`](../features/std/cpp20/cpp20_consteval.cpp) /
   [`cpp20_constinit`](../features/std/cpp20/cpp20_constinit.cpp) /
   [`cpp20_is_constant_evaluated`](../features/std/cpp20/cpp20_is_constant_evaluated.cpp)
   — C++20 splits intent three ways: *must* run at compile time (consteval),
   *must initialise* at compile time but stay mutable (constinit), and "am I
   currently constant-evaluated?" for dual-path functions.
5. [`cpp23_if_consteval`](../features/std/cpp23/cpp23_if_consteval.cpp) — the
   language form that supersedes `std::is_constant_evaluated()` and composes
   correctly with consteval.
6. [`cpp26_static_assert_messages`](../features/std/cpp26/cpp26_static_assert_messages.cpp)
   + [`cpp26_pack_indexing`](../features/std/cpp26/cpp26_pack_indexing.cpp) —
   diagnostics and pack access become compile-time computed values;
   [`cpp26_reflection_basic`](../features/std/cpp26/cpp26_reflection_basic.cpp)
   points at where this is heading.

## Lambdas and callables

1. [`cpp11_lambda`](../features/std/cpp11/cpp11_lambda.cpp),
   [`cpp11_function`](../features/std/cpp11/cpp11_function.cpp),
   [`cpp11_bind`](../features/std/cpp11/cpp11_bind.cpp) — closures arrive; the
   type-erased `std::function` and the placeholder-based `std::bind`.
2. [`cpp14_generic_lambda`](../features/std/cpp14/cpp14_generic_lambda.cpp) +
   [`cpp14_lambda_init_capture`](../features/std/cpp14/cpp14_lambda_init_capture.cpp)
   — `auto` parameters make one lambda work for many types; init-captures
   finally allow *moving* into a closure.
3. [`cpp17_not_fn_as_const`](../features/std/cpp17/cpp17_not_fn_as_const.cpp)
   — the binder cleanup begins (`not1`/`not2` die).
4. [`cpp20_lambdas`](../features/std/cpp20/cpp20_lambdas.cpp) +
   [`cpp20_bind_front`](../features/std/cpp20/cpp20_bind_front.cpp) —
   template parameter lists on lambdas; `bind_front` replaces almost every
   remaining `std::bind` use.
5. [`cpp23_deducing_this`](../features/std/cpp23/cpp23_deducing_this.cpp) /
   [`cpp23_recursive_lambda`](../features/std/cpp23/cpp23_recursive_lambda.cpp)
   / [`cpp23_move_only_function`](../features/std/cpp23/cpp23_move_only_function.cpp)
   / [`cpp23_static_operator`](../features/std/cpp23/cpp23_static_operator.cpp)
   — the explicit object parameter removes CRTP and overload duplication,
   lambdas can recurse, and the type-erased wrapper learns about move-only
   callables.

## From loops to ranges

1. [`cpp11_range_for`](../features/std/cpp11/cpp11_range_for.cpp) +
   [`cpp11_iterator_helpers`](../features/std/cpp11/cpp11_iterator_helpers.cpp)
   + [`cpp11_algorithms_new`](../features/std/cpp11/cpp11_algorithms_new.cpp)
   — range-for and free `begin`/`end` make iteration uniform.
2. [`cpp14_cbegin_cend`](../features/std/cpp14/cpp14_cbegin_cend.cpp) +
   [`cpp14_dual_range_algorithms`](../features/std/cpp14/cpp14_dual_range_algorithms.cpp)
   — const iteration and the two-range algorithm overloads close C++11's
   sharp edges.
3. [`cpp17_parallel_algos`](../features/std/cpp17/cpp17_parallel_algos.cpp) —
   the same algorithms, parallel execution policies.
4. [`cpp20_ranges_algorithms`](../features/std/cpp20/cpp20_ranges_algorithms.cpp)
   + [`cpp20_ranges_views`](../features/std/cpp20/cpp20_ranges_views.cpp) +
   [`cpp20_views_split`](../features/std/cpp20/cpp20_views_split.cpp) —
   algorithms take whole ranges + projections; views compose lazily.
5. [`cpp23_ranges_to`](../features/std/cpp23/cpp23_ranges_to.cpp),
   [`cpp23_ranges_fold`](../features/std/cpp23/cpp23_ranges_fold.cpp),
   [`cpp23_views_chunk_by`](../features/std/cpp23/cpp23_views_chunk_by.cpp),
   [`cpp23_ranges_zip`](../features/std/cpp23/cpp23_ranges_zip.cpp), and
   friends — the view/algorithm vocabulary roughly doubles and materialising
   results becomes a one-liner.

## Value types for errors and alternatives

1. [`cpp11_exception_ptr`](../features/std/cpp11/cpp11_exception_ptr.cpp) +
   [`cpp11_system_error`](../features/std/cpp11/cpp11_system_error.cpp) —
   errors as values, first attempt: transportable exceptions and
   `error_code`.
2. [`cpp17_optional`](../features/std/cpp17/cpp17_optional.cpp) /
   [`cpp17_variant`](../features/std/cpp17/cpp17_variant.cpp) /
   [`cpp17_any`](../features/std/cpp17/cpp17_any.cpp) — nullable, sum, and
   open-set types make "no result" and "one of N" explicit.
3. [`cpp23_expected`](../features/std/cpp23/cpp23_expected.cpp) +
   [`cpp23_optional_monadic`](../features/std/cpp23/cpp23_optional_monadic.cpp)
   — value-or-error with monadic chaining; `and_then`/`transform`/`or_else`
   replace nested ifs.
4. [`cpp26_contracts_basic`](../features/std/cpp26/cpp26_contracts_basic.cpp)
   (experimental) — pre/postconditions move into the language itself.

## Concurrency

1. [`cpp11_thread`](../features/std/cpp11/cpp11_thread.cpp),
   [`cpp11_thread_local`](../features/std/cpp11/cpp11_thread_local.cpp),
   [`cpp11_mutex_lock`](../features/std/cpp11/cpp11_mutex_lock.cpp),
   [`cpp11_condition_variable`](../features/std/cpp11/cpp11_condition_variable.cpp),
   [`cpp11_atomic`](../features/std/cpp11/cpp11_atomic.cpp),
   [`cpp11_async_future`](../features/std/cpp11/cpp11_async_future.cpp) — the
   foundation: threads, per-thread storage, locks, atomics, and a first stab
   at tasks.
2. [`cpp14_shared_timed_mutex`](../features/std/cpp14/cpp14_shared_timed_mutex.cpp)
   — many readers / one writer, with deadlines.
3. [`cpp17_shared_mutex`](../features/std/cpp17/cpp17_shared_mutex.cpp) +
   [`cpp17_scoped_lock`](../features/std/cpp17/cpp17_scoped_lock.cpp) — the
   untimed reader-writer lock and deadlock-free multi-mutex RAII.
4. [`cpp20_jthread`](../features/std/cpp20/cpp20_jthread.cpp) +
   [`cpp20_stop_token`](../features/std/cpp20/cpp20_stop_token.cpp), the
   semaphores/[`cpp20_latch`](../features/std/cpp20/cpp20_latch.cpp)/
   [`cpp20_barrier`](../features/std/cpp20/cpp20_barrier.cpp),
   [`cpp20_atomic_wait`](../features/std/cpp20/cpp20_atomic_wait.cpp),
   [`cpp20_atomic_ref`](../features/std/cpp20/cpp20_atomic_ref.cpp),
   [`cpp20_syncstream`](../features/std/cpp20/cpp20_syncstream.cpp) —
   cooperative cancellation and the complete synchronisation toolbox.

## Text, formatting, and constants

1. [`cpp11_raw_string`](../features/std/cpp11/cpp11_raw_string.cpp) +
   [`cpp11_user_defined_literal`](../features/std/cpp11/cpp11_user_defined_literal.cpp)
   — literals get powers.
2. [`cpp14_std_literals`](../features/std/cpp14/cpp14_std_literals.cpp) +
   [`cpp14_quoted`](../features/std/cpp14/cpp14_quoted.cpp) — the standard
   library ships its own suffixes (`"s"`, `1500ms`) and round-trippable
   quoting.
3. [`cpp17_string_view`](../features/std/cpp17/cpp17_string_view.cpp) +
   [`cpp17_charconv`](../features/std/cpp17/cpp17_charconv.cpp) — non-owning
   text and locale-free, allocation-free number conversion.
4. [`cpp20_format`](../features/std/cpp20/cpp20_format.cpp) — Python-style,
   type-safe formatting.
5. [`cpp23_print`](../features/std/cpp23/cpp23_print.cpp) +
   [`cpp23_format_ranges`](../features/std/cpp23/cpp23_format_ranges.cpp) —
   direct printing, and every container becomes formattable.
6. [`cpp26_text_encoding`](../features/std/cpp26/cpp26_text_encoding.cpp) —
   the encoding of your literals stops being folklore and becomes queryable.

## Generic programming

1. [`cpp11_variadic_templates`](../features/std/cpp11/cpp11_variadic_templates.cpp)
   + [`cpp11_type_traits`](../features/std/cpp11/cpp11_type_traits.cpp) —
   packs and the traits vocabulary.
2. [`cpp14_variable_templates`](../features/std/cpp14/cpp14_variable_templates.cpp)
   + [`cpp14_integer_sequence`](../features/std/cpp14/cpp14_integer_sequence.cpp)
   — templated constants (the `_v` pattern) and the canonical pack-expansion
   tool.
3. [`cpp17_fold_expressions`](../features/std/cpp17/cpp17_fold_expressions.cpp)
   + [`cpp17_ctad_basic`](../features/std/cpp17/cpp17_ctad_basic.cpp) +
   [`cpp17_type_trait_helpers`](../features/std/cpp17/cpp17_type_trait_helpers.cpp)
   — packs reduce in one expression; constructor calls deduce their types.
4. [`cpp20_concepts_intro`](../features/std/cpp20/cpp20_concepts_intro.cpp) +
   [`cpp20_concepts_requires_expr`](../features/std/cpp20/cpp20_concepts_requires_expr.cpp)
   + [`cpp20_abbreviated_templates`](../features/std/cpp20/cpp20_abbreviated_templates.cpp)
   + [`cpp20_nttp_class`](../features/std/cpp20/cpp20_nttp_class.cpp) —
   constraints replace SFINAE, and templates start reading like functions.
5. [`cpp23_deducing_this`](../features/std/cpp23/cpp23_deducing_this.cpp) —
   the library-building idioms (CRTP, quadruplicated overloads) collapse
   into one mechanism.
6. [`cpp26_pack_indexing`](../features/std/cpp26/cpp26_pack_indexing.cpp) —
   direct pack element access;
   [`cpp26_reflection_basic`](../features/std/cpp26/cpp26_reflection_basic.cpp)
   previews the endgame.

## Reading advice

Each storyline is deliberately short — three to six files read in order, most
under a screen of code. If a step surprises you, the per-bucket README
(one directory up from each file) has the one-line summary of every sibling
example, and [docs/gcc-changelogs.md](gcc-changelogs.md) tells you which GCC
release made each step usable in practice.
