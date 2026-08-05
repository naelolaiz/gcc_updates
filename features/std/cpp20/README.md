# C++20 examples

_Folder: `features/std/cpp20/`. 9 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [chrono](#chrono)
- [concepts](#concepts)
- [coroutines](#coroutines)
- [format](#format)
- [language](#language)
- [ranges](#ranges)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)

## chrono

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_chrono_calendar.cpp](cpp20_chrono_calendar.cpp) | c++20 | 14 | <chrono> grew calendar types: year_month_day, weekday, sys_days, ymd_weekday arithmetic. |

## concepts

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_concepts_intro.cpp](cpp20_concepts_intro.cpp) | c++20 | 14 | Defining and using concepts to constrain templates -- replaces SFINAE for most cases. |
| [cpp20_concepts_requires_expr.cpp](cpp20_concepts_requires_expr.cpp) | c++20 | 14 | requires-expressions can introspect type capabilities (simple, type, compound, nested requirements). |

## coroutines

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_coroutine_generator.cpp](cpp20_coroutine_generator.cpp) | c++20 | 14 | Hand-rolled minimal generator using <coroutine> primitives -- co_yield, promise_type, coroutine_handle. |

## format

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_format.cpp](cpp20_format.cpp) | c++20 | 14 | std::format provides Python-style typesafe formatting; std::format_to writes into an output iterator. |

## language

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_consteval.cpp](cpp20_consteval.cpp) | c++20 | 14 | consteval functions MUST be evaluated at compile time; constinit guarantees static-init at compile time. |
| [cpp20_designated_init.cpp](cpp20_designated_init.cpp) | c++20 | 14 | Designated initializers let you initialise aggregate members by name; order must match declaration. |
| [cpp20_lambdas.cpp](cpp20_lambdas.cpp) | c++20 | 14 | C++20 added template parameter lists on lambdas, default-construction, and capture of *this by value. |
| [cpp20_spaceship.cpp](cpp20_spaceship.cpp) | c++20 | 14 | operator<=> ("spaceship") generates the full set of relational operators from one declaration. |

## ranges

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_ranges_algorithms.cpp](cpp20_ranges_algorithms.cpp) | c++20 | 14 | std::ranges versions of classic algorithms accept full ranges + projections directly. |
| [cpp20_ranges_views.cpp](cpp20_ranges_views.cpp) | c++20 | 14 | Lazy view composition with views::filter, views::transform and views::take using the | pipe. |
| [cpp20_views_keys_values.cpp](cpp20_views_keys_values.cpp) | c++20 | 13 | views::keys / views::values / views::elements project tuple-like ranges to a single component. |

## stl

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_bit_ops.cpp](cpp20_bit_ops.cpp) | c++20 | 14 | <bit> standardises common bit tricks: popcount, countl_zero, has_single_bit, bit_ceil, bit_cast. |
| [cpp20_endian.cpp](cpp20_endian.cpp) | c++20 | 13 | std::endian exposes the host byte order at compile time -- portable replacement for compiler-specific macros. |
| [cpp20_erase_if.cpp](cpp20_erase_if.cpp) | c++20 | 14 | std::erase / std::erase_if removes the erase-remove idiom for every standard container. |
| [cpp20_lerp_midpoint.cpp](cpp20_lerp_midpoint.cpp) | c++20 | 14 | std::lerp and std::midpoint give numerically-careful linear interpolation and midpoint, including for integers. |
| [cpp20_map_contains.cpp](cpp20_map_contains.cpp) | c++20 | 14 | Associative containers gained .contains(key) -- clearer than 'count(k) > 0' or 'find(k) != end()'. |
| [cpp20_numbers.cpp](cpp20_numbers.cpp) | c++20 | 14 | <numbers> ships precise math constants typed as templates; no more M_PI macros. |
| [cpp20_source_location.cpp](cpp20_source_location.cpp) | c++20 | 14 | std::source_location replaces __FILE__/__LINE__/__func__ macros with a real value type. |
| [cpp20_span.cpp](cpp20_span.cpp) | c++20 | 14 | std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind. |
| [cpp20_ssize.cpp](cpp20_ssize.cpp) | c++20 | 14 | std::ssize returns a SIGNED size, eliminating the int/size_t mixed-comparison warnings of c.size(). |
| [cpp20_string_starts_ends.cpp](cpp20_string_starts_ends.cpp) | c++20 | 14 | std::string and std::string_view gained starts_with / ends_with member functions. |
| [cpp20_to_address.cpp](cpp20_to_address.cpp) | c++20 | 13 | std::to_address turns any 'fancy pointer' (smart, iterator) into a raw pointer without dereferencing. |

## templates

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_ctad_aggregates.cpp](cpp20_ctad_aggregates.cpp) | c++20 | 14 | Class template argument deduction now works on aggregates without writing deduction guides by hand. |
| [cpp20_ctad_alias.cpp](cpp20_ctad_alias.cpp) | c++20 | 14 | C++20 lets CTAD work through alias templates -- no manual deduction guide needed. |
| [cpp20_explicit_bool.cpp](cpp20_explicit_bool.cpp) | c++20 | 14 | explicit(bool) makes a constructor's explicitness depend on a constexpr predicate. |
| [cpp20_nttp_class.cpp](cpp20_nttp_class.cpp) | c++20 | 14 | Class types can now be non-type template parameters (NTTPs); enables compile-time string templates. |
| [cpp20_type_identity.cpp](cpp20_type_identity.cpp) | c++20 | 13 | C++20 added std::type_identity<T> (block template argument deduction in non-deduced contexts) and std::remove_cvref<T> (the common 'strip cv + reference, but not array-to-pointer decay' transformation). |

## threading

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [cpp20_atomic_flag_test.cpp](cpp20_atomic_flag_test.cpp) | c++20 | 14 | C++20 added test()/wait()/notify_one to std::atomic_flag, finally making it useful beyond test_and_set. |
| [cpp20_atomic_ref.cpp](cpp20_atomic_ref.cpp) | c++20 | 14 | std::atomic_ref overlays atomic semantics on a non-atomic object you don't own. |
| [cpp20_atomic_shared_ptr.cpp](cpp20_atomic_shared_ptr.cpp) | c++20 | 14 | std::atomic<std::shared_ptr<T>> -- lock-free-style atomic load/store/exchange of shared ownership. |
| [cpp20_atomic_wait.cpp](cpp20_atomic_wait.cpp) | c++20 | 14 | std::atomic<T>::wait/notify_one/notify_all enable lock-free condition-variable-style blocking. |
| [cpp20_barrier.cpp](cpp20_barrier.cpp) | c++20 | 14 | std::barrier is reusable across phases and runs an optional completion function between phases. |
| [cpp20_binary_semaphore.cpp](cpp20_binary_semaphore.cpp) | c++20 | 14 | std::binary_semaphore = counting_semaphore<1>; classic single-slot signalling primitive. |
| [cpp20_counting_semaphore.cpp](cpp20_counting_semaphore.cpp) | c++20 | 14 | std::counting_semaphore caps concurrent access to a fixed number of "permits". |
| [cpp20_jthread.cpp](cpp20_jthread.cpp) | c++20 | 14 | std::jthread auto-joins on destruction and supports cooperative cancellation via stop_token. |
| [cpp20_latch.cpp](cpp20_latch.cpp) | c++20 | 14 | std::latch is a one-shot countdown synchroniser; threads wait until it hits zero. |
| [cpp20_stop_token.cpp](cpp20_stop_token.cpp) | c++20 | 14 | std::stop_source / stop_token / stop_callback decouple cancellation request from the threads observing it. |
| [cpp20_syncstream.cpp](cpp20_syncstream.cpp) | c++20 | 14 | std::osyncstream interleaves output safely from multiple threads -- each scope's text is emitted as one block. |
