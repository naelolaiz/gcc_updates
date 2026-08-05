# C++23 examples

_Folder: `features/std/cpp23/`. 5 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [coroutines](#coroutines)
- [language](#language)
- [ranges](#ranges)
- [stl](#stl)
- [templates](#templates)

## coroutines

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp23_generator.cpp](cpp23_generator.cpp) | c++23 | 14 | std::generator is the standard library's coroutine generator -- a range-compatible co_yield producer. |

## language

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp23_assume.cpp](cpp23_assume.cpp) | c++23 | 14 | [[assume(expr)]] tells the optimizer 'expr is true here' WITHOUT evaluating it -- a portable hint. |
| [cpp23_auto_decay_copy.cpp](cpp23_auto_decay_copy.cpp) | c++23 | 14 | auto(x) and auto{x} produce a decayed prvalue copy of x -- handy for materialising a copy in generic code. |
| [cpp23_if_consteval.cpp](cpp23_if_consteval.cpp) | c++23 | 14 | 'if consteval' is the standardised, simpler replacement for std::is_constant_evaluated(). |
| [cpp23_size_t_literal.cpp](cpp23_size_t_literal.cpp) | c++23 | 14 | 1uz / 1z literal suffix yields a std::size_t / std::ptrdiff_t directly; no more static_cast<size_t>(0). |

## ranges

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp23_ranges_adjacent.cpp](cpp23_ranges_adjacent.cpp) | c++23 | 14 | views::adjacent<N> yields N-tuples of consecutive elements; pairwise = adjacent<2>. |
| [cpp23_ranges_cartesian_product.cpp](cpp23_ranges_cartesian_product.cpp) | c++23 | 14 | views::cartesian_product yields the Cartesian product of N input ranges as tuples. |
| [cpp23_ranges_chunk_slide.cpp](cpp23_ranges_chunk_slide.cpp) | c++23 | 14 | views::chunk splits a range into non-overlapping windows of size N; views::slide does overlapping windows. |
| [cpp23_ranges_enumerate.cpp](cpp23_ranges_enumerate.cpp) | c++23 | 14 | views::enumerate yields (index, value) pairs -- the standard equivalent of Python's enumerate(). |
| [cpp23_ranges_find_last.cpp](cpp23_ranges_find_last.cpp) | c++23 | 14 | ranges::find_last / find_last_if / find_last_if_not return the LAST matching position; previously you reversed first. |
| [cpp23_ranges_fold.cpp](cpp23_ranges_fold.cpp) | c++23 | 14 | ranges::fold_left / fold_right / fold_left_first replace std::accumulate; concept-checked, projection-aware. |
| [cpp23_ranges_iota_algorithm.cpp](cpp23_ranges_iota_algorithm.cpp) | c++23 | 14 | std::ranges::iota (the algorithm, not the view) fills a range with sequentially increasing values; concept-checked. |
| [cpp23_ranges_join_with.cpp](cpp23_ranges_join_with.cpp) | c++23 | 14 | views::join_with flattens a range-of-ranges with a separator (single element OR a range). |
| [cpp23_ranges_starts_ends_contains.cpp](cpp23_ranges_starts_ends_contains.cpp) | c++23 | 14 | ranges::starts_with / ends_with / contains / contains_subrange -- algorithm form of the string-shaped predicates. |
| [cpp23_ranges_to.cpp](cpp23_ranges_to.cpp) | c++23 | 14 | std::ranges::to materialises a view (or any range) into a concrete container in one expression. |
| [cpp23_ranges_zip.cpp](cpp23_ranges_zip.cpp) | c++23 | 14 | std::views::zip turns parallel ranges into a single range of tuples; stops at shortest input. |
| [cpp23_views_as_const_as_rvalue.cpp](cpp23_views_as_const_as_rvalue.cpp) | c++23 | 14 | views::as_const yields const references; views::as_rvalue yields rvalue references (enables move-iteration). |
| [cpp23_views_repeat.cpp](cpp23_views_repeat.cpp) | c++23 | 14 | views::repeat yields the same value forever (or N times); composes with views::take for finite ranges. |
| [cpp23_views_stride.cpp](cpp23_views_stride.cpp) | c++23 | 14 | views::stride takes every N-th element; useful for downsampling. |

## stl

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp23_byteswap.cpp](cpp23_byteswap.cpp) | c++23 | 14 | std::byteswap reverses the byte order of an integer; constexpr-friendly endian conversion. |
| [cpp23_expected.cpp](cpp23_expected.cpp) | c++23 | 14 | std::expected<T,E> is a value-or-error type with monadic and_then / or_else / transform. |
| [cpp23_flat_map.cpp](cpp23_flat_map.cpp) | c++23 | 15 | std::flat_map keeps two parallel sorted vectors instead of a tree -- cache-friendly, lookup O(log n), insert O(n). |
| [cpp23_flat_set.cpp](cpp23_flat_set.cpp) | c++23 | 15 | std::flat_set is the set-shaped counterpart of flat_map: sorted contiguous storage instead of an RB tree. |
| [cpp23_format_ranges.cpp](cpp23_format_ranges.cpp) | c++23 | 14 | C++23 made every range formattable by std::format -- pass any container/view through "{}", customise per-element with the nested format spec "{::}", or pick join_with for a custom separator. |
| [cpp23_forward_like.cpp](cpp23_forward_like.cpp) | c++23 | 14 | std::forward_like<Self>(x) returns x with the cv-qualification AND value category of Self -- the cleanest way to forward an inner member from a deducing-this method without writing four overloads. |
| [cpp23_mdspan.cpp](cpp23_mdspan.cpp) | c++23 | 16 | std::mdspan is a non-owning view over multi-dimensional contiguous data; supports static and dynamic extents. |
| [cpp23_move_only_function.cpp](cpp23_move_only_function.cpp) | c++23 | 14 | std::move_only_function is std::function but for move-only callables (e.g. unique_ptr captures, std::packaged_task-style ownership). |
| [cpp23_optional_monadic.cpp](cpp23_optional_monadic.cpp) | c++23 | 14 | std::optional gained monadic ops (and_then / transform / or_else) -- chain fallible computations cleanly. |
| [cpp23_out_ptr.cpp](cpp23_out_ptr.cpp) | c++23 | 14 | std::out_ptr / std::inout_ptr adapt smart pointers to legacy 'T**' APIs without manual reset() dancing. |
| [cpp23_print.cpp](cpp23_print.cpp) | c++23 | 14 | std::print / std::println write directly to stdout using std::format syntax. On GCC 14 the impl lives in libstdc++exp. |
| [cpp23_resize_and_overwrite.cpp](cpp23_resize_and_overwrite.cpp) | c++23 | 14 | std::string::resize_and_overwrite gives you an uninitialized buffer of N bytes, then trusts your callback to size it down. |
| [cpp23_spanstream.cpp](cpp23_spanstream.cpp) | c++23 | 14 | <spanstream> wraps a std::span as the buffer for an iostream -- ispanstream / ospanstream / spanstream do the same job as stringstream but with no allocation, no copy, and no implicit lifetime ownership. |
| [cpp23_stacktrace.cpp](cpp23_stacktrace.cpp) | c++23 | 14 | std::stacktrace captures and pretty-prints the current call stack; symbol names depend on debuginfo. |
| [cpp23_start_lifetime_as.cpp](cpp23_start_lifetime_as.cpp) | c++23 | 16 | std::start_lifetime_as<T>(p) formally begins T's lifetime at the bytes pointed to by p without running a constructor -- the standardised, defined-behaviour way to "view a buffer as a T", obsoleting the reinterpret_cast / memcpy dance for trivially-copyable types. |
| [cpp23_string_contains.cpp](cpp23_string_contains.cpp) | c++23 | 14 | std::string and std::string_view gained .contains() in C++23 (starts_with/ends_with were C++20). |
| [cpp23_to_underlying.cpp](cpp23_to_underlying.cpp) | c++23 | 14 | std::to_underlying returns the underlying-type value of an enum -- replaces the static_cast boilerplate. |
| [cpp23_unreachable.cpp](cpp23_unreachable.cpp) | c++23 | 14 | std::unreachable() marks code paths that cannot be reached -- the optimizer assumes UB if they ever are. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp23_deducing_this.cpp](cpp23_deducing_this.cpp) | c++23 | 14 | Explicit object parameter ('deducing this') replaces ref-qual overload sets and enables CRTP-without-CRTP. |
| [cpp23_multidim_subscript.cpp](cpp23_multidim_subscript.cpp) | c++23 | 14 | operator[] now accepts multiple arguments -- the natural syntax for tensor / matrix types. |
| [cpp23_recursive_lambda.cpp](cpp23_recursive_lambda.cpp) | c++23 | 14 | Deducing this lets a lambda recurse into itself without std::function or fixed-point combinator gymnastics. |
| [cpp23_static_operator.cpp](cpp23_static_operator.cpp) | c++23 | 14 | operator() and operator[] can be 'static' -- no implicit object parameter, slightly cheaper to call. |
