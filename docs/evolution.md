# How C++ features evolved across standards

The per-bucket indexes answer "what's in C++NN?". This page answers the other
question: "how did one idea grow from C++11 to C++26?" Each section names the
examples (all runnable via `ctest -R <name>`) that form one storyline, in the
order the language acquired them. For a flat per-topic listing across buckets,
see [features/TOPICS.md](../features/TOPICS.md).

## Compile-time programming

The single biggest arc in modern C++: from "constants with functions attached"
to a second, full programming language that runs inside the compiler.

1. `cpp11_constexpr` — C++11 constexpr functions are a single return
   statement; recursion is the only loop.
2. `cpp14_relaxed_constexpr` — loops, branches, and local mutation become
   legal; real algorithms move to compile time.
3. `cpp17_if_constexpr` + `cpp17_constexpr_lambda` — compile-time *branching
   on types* inside one template, and lambdas join the party.
4. `cpp20_consteval` / `cpp20_constinit` / `cpp20_is_constant_evaluated` —
   C++20 splits intent three ways: *must* run at compile time (consteval),
   *must initialise* at compile time but stay mutable (constinit), and "am I
   currently constant-evaluated?" for dual-path functions.
5. `cpp23_if_consteval` — the language form that supersedes
   `std::is_constant_evaluated()` and composes correctly with consteval.
6. `cpp26_static_assert_messages` + `cpp26_pack_indexing` — diagnostics and
   pack access become compile-time computed values;
   `cpp26_reflection_basic` points at where this is heading.

## Lambdas and callables

1. `cpp11_lambda`, `cpp11_function`, `cpp11_bind` — closures arrive; the
   type-erased `std::function` and the placeholder-based `std::bind`.
2. `cpp14_generic_lambda` + `cpp14_lambda_init_capture` — `auto` parameters
   make one lambda work for many types; init-captures finally allow *moving*
   into a closure.
3. `cpp17_not_fn_as_const` — the binder cleanup begins (`not1`/`not2` die).
4. `cpp20_lambdas` + `cpp20_bind_front` — template parameter lists on
   lambdas; `bind_front` replaces almost every remaining `std::bind` use.
5. `cpp23_deducing_this` / `cpp23_recursive_lambda` /
   `cpp23_move_only_function` / `cpp23_static_operator` — the explicit object
   parameter removes CRTP and overload duplication, lambdas can recurse, and
   the type-erased wrapper learns about move-only callables.

## From loops to ranges

1. `cpp11_range_for` + `cpp11_iterator_helpers` + `cpp11_algorithms_new` —
   range-for and free `begin`/`end` make iteration uniform.
2. `cpp14_cbegin_cend` + `cpp14_dual_range_algorithms` — const iteration and
   the two-range algorithm overloads close C++11's sharp edges.
3. `cpp17_parallel_algos` — the same algorithms, parallel execution policies.
4. `cpp20_ranges_algorithms` + `cpp20_ranges_views` + `cpp20_views_split` —
   algorithms take whole ranges + projections; views compose lazily.
5. `cpp23_ranges_to`, `cpp23_ranges_fold`, `cpp23_views_chunk_by`,
   `cpp23_ranges_zip`, and friends — the view/algorithm vocabulary roughly
   doubles and materialising results becomes a one-liner.

## Value types for errors and alternatives

1. `cpp11_exception_ptr` + `cpp11_system_error` — errors as values, first
   attempt: transportable exceptions and `error_code`.
2. `cpp17_optional` / `cpp17_variant` / `cpp17_any` — nullable, sum, and
   open-set types make "no result" and "one of N" explicit.
3. `cpp23_expected` + `cpp23_optional_monadic` — value-or-error with monadic
   chaining; `and_then`/`transform`/`or_else` replace nested ifs.
4. `cpp26_contracts_basic` (experimental) — pre/postconditions move into the
   language itself.

## Concurrency

1. `cpp11_thread`, `cpp11_mutex_lock`, `cpp11_condition_variable`,
   `cpp11_atomic`, `cpp11_async_future` — the foundation: threads, locks,
   atomics, and a first stab at tasks.
2. `cpp14_shared_timed_mutex` — many readers / one writer, with deadlines.
3. `cpp17_shared_mutex` + `cpp17_scoped_lock` — the untimed reader-writer
   lock and deadlock-free multi-mutex RAII.
4. `cpp20_jthread` + `cpp20_stop_token`, the semaphores/`cpp20_latch`/
   `cpp20_barrier`, `cpp20_atomic_wait`, `cpp20_atomic_ref`,
   `cpp20_syncstream` — cooperative cancellation and the complete
   synchronisation toolbox.

## Text, formatting, and constants

1. `cpp11_raw_string` + `cpp11_user_defined_literal` — literals get powers.
2. `cpp14_std_literals` + `cpp14_quoted` — the standard library ships its own
   suffixes (`"s"`, `1500ms`) and round-trippable quoting.
3. `cpp17_string_view` + `cpp17_charconv` — non-owning text and locale-free,
   allocation-free number conversion.
4. `cpp20_format` — Python-style, type-safe formatting.
5. `cpp23_print` + `cpp23_format_ranges` — direct printing, and every
   container becomes formattable.
6. `cpp26_text_encoding` — the encoding of your literals stops being folklore
   and becomes queryable.

## Generic programming

1. `cpp11_variadic_templates` + `cpp11_type_traits` — packs and the traits
   vocabulary.
2. `cpp14_variable_templates` + `cpp14_integer_sequence` — templated
   constants (the `_v` pattern) and the canonical pack-expansion tool.
3. `cpp17_fold_expressions` + `cpp17_ctad_basic` + `cpp17_type_trait_helpers`
   — packs reduce in one expression; constructor calls deduce their types.
4. `cpp20_concepts_intro` + `cpp20_concepts_requires_expr` +
   `cpp20_abbreviated_templates` + `cpp20_nttp_class` — constraints replace
   SFINAE, and templates start reading like functions.
5. `cpp23_deducing_this` — the library-building idioms (CRTP, quadruplicated
   overloads) collapse into one mechanism.
6. `cpp26_pack_indexing` — direct pack element access;
   `cpp26_reflection_basic` previews the endgame.

## Reading advice

Each storyline is deliberately short — three to six files read in order, most
under a screen of code. If a step surprises you, the per-bucket README
(one directory up from each file) has the one-line summary of every sibling
example, and [docs/gcc-changelogs.md](gcc-changelogs.md) tells you which GCC
release made each step usable in practice.
