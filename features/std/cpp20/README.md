# C++20 examples

_Folder: `features/std/cpp20/`. 20 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/podman-dev.sh <ver> readme`._

## Topics

- [algorithms](#algorithms)
- [build-system](#build-system)
- [cancellation](#cancellation)
- [chrono](#chrono)
- [concepts](#concepts)
- [coroutines](#coroutines)
- [diagnostics](#diagnostics)
- [essential](#essential)
- [format](#format)
- [initialization](#initialization)
- [io](#io)
- [iterators](#iterators)
- [language](#language)
- [lifetimes](#lifetimes)
- [modules](#modules)
- [ranges](#ranges)
- [stl](#stl)
- [templates](#templates)
- [threading](#threading)
- [views](#views)

## algorithms

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_ranges_views.cpp](cpp20_ranges_views.cpp) | c++20 | GCC >= 14 | covered | Lazy view composition with views::filter, views::transform and views::take using the | pipe. |

## build-system

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_modules_basic.cpp](cpp20_modules_basic.cpp) | c++20 | GCC >= 16 | covered | C++20 modules compile an interface before an importer; this two-file fixture proves export, import, link, and execution with GCC's modules implementation. |

## cancellation

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_jthread.cpp](cpp20_jthread.cpp) | c++20 | GCC >= 14 | covered | std::jthread auto-joins on destruction and supports cooperative cancellation via stop_token. |

## chrono

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_chrono_calendar.cpp](cpp20_chrono_calendar.cpp) | c++20 | GCC >= 14 | covered | <chrono> grew calendar types: year_month_day, weekday, sys_days, ymd_weekday arithmetic. |

## concepts

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_concepts_constraint_error.cpp](cpp20_concepts_constraint_error.cpp) | c++20 | GCC >= 14 | negative | A constrained overload rejects a non-integral argument at selection time and produces a constraints-not-satisfied diagnostic. |
| [cpp20_concepts_intro.cpp](cpp20_concepts_intro.cpp) | c++20 | GCC >= 14 | covered | Defining and using concepts to constrain templates -- replaces SFINAE for most cases. |
| [cpp20_concepts_requires_expr.cpp](cpp20_concepts_requires_expr.cpp) | c++20 | GCC >= 14 | covered | requires-expressions can introspect type capabilities (simple, type, compound, nested requirements). |

## coroutines

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_coroutine_generator.cpp](cpp20_coroutine_generator.cpp) | c++20 | GCC >= 14 | covered | Hand-rolled minimal generator using <coroutine> primitives -- co_yield, promise_type, coroutine_handle. |

## diagnostics

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_concepts_constraint_error.cpp](cpp20_concepts_constraint_error.cpp) | c++20 | GCC >= 14 | negative | A constrained overload rejects a non-integral argument at selection time and produces a constraints-not-satisfied diagnostic. |
| [cpp20_concepts_intro.cpp](cpp20_concepts_intro.cpp) | c++20 | GCC >= 14 | covered | Defining and using concepts to constrain templates -- replaces SFINAE for most cases. |
| [cpp20_designated_init_order_error.cpp](cpp20_designated_init_order_error.cpp) | c++20 | GCC >= 14 | negative | C++20 designated initializers must follow declaration order; this negative example asserts that portability rule. |

## essential

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_concepts_intro.cpp](cpp20_concepts_intro.cpp) | c++20 | GCC >= 14 | covered | Defining and using concepts to constrain templates -- replaces SFINAE for most cases. |
| [cpp20_format.cpp](cpp20_format.cpp) | c++20 | GCC >= 14 | covered | std::format provides Python-style typesafe formatting; std::format_to writes into an output iterator. |
| [cpp20_jthread.cpp](cpp20_jthread.cpp) | c++20 | GCC >= 14 | covered | std::jthread auto-joins on destruction and supports cooperative cancellation via stop_token. |
| [cpp20_modules_basic.cpp](cpp20_modules_basic.cpp) | c++20 | GCC >= 16 | covered | C++20 modules compile an interface before an importer; this two-file fixture proves export, import, link, and execution with GCC's modules implementation. |
| [cpp20_ranges_views.cpp](cpp20_ranges_views.cpp) | c++20 | GCC >= 14 | covered | Lazy view composition with views::filter, views::transform and views::take using the | pipe. |
| [cpp20_span.cpp](cpp20_span.cpp) | c++20 | GCC >= 14 | covered | std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind. |

## format

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_format.cpp](cpp20_format.cpp) | c++20 | GCC >= 14 | covered | std::format provides Python-style typesafe formatting; std::format_to writes into an output iterator. |

## initialization

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_designated_init_order_error.cpp](cpp20_designated_init_order_error.cpp) | c++20 | GCC >= 14 | negative | C++20 designated initializers must follow declaration order; this negative example asserts that portability rule. |

## io

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_format.cpp](cpp20_format.cpp) | c++20 | GCC >= 14 | covered | std::format provides Python-style typesafe formatting; std::format_to writes into an output iterator. |

## iterators

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_ranges_borrowed_range.cpp](cpp20_ranges_borrowed_range.cpp) | c++20 | GCC >= 14 | covered | borrowed_range explains why an algorithm result from a temporary can be an iterator for span but std::ranges::dangling for an owning vector. |

## language

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_consteval.cpp](cpp20_consteval.cpp) | c++20 | GCC >= 14 | covered | consteval functions MUST be evaluated at compile time; constinit guarantees static-init at compile time. |
| [cpp20_constinit.cpp](cpp20_constinit.cpp) | c++20 | GCC >= 13 | covered | constinit forces a static/thread_local variable to be initialised at compile time (killing static-init-order surprises) while staying mutable at runtime -- unlike constexpr, which also makes it const. |
| [cpp20_designated_init.cpp](cpp20_designated_init.cpp) | c++20 | GCC >= 14 | covered | Designated initializers let you initialise aggregate members by name; order must match declaration. |
| [cpp20_designated_init_order_error.cpp](cpp20_designated_init_order_error.cpp) | c++20 | GCC >= 14 | negative | C++20 designated initializers must follow declaration order; this negative example asserts that portability rule. |
| [cpp20_is_constant_evaluated.cpp](cpp20_is_constant_evaluated.cpp) | c++20 | GCC >= 13 | covered | std::is_constant_evaluated() lets one constexpr function take a compile-time-safe path during constant evaluation and a faster/library path at runtime (C++23's 'if consteval' supersedes the pattern). |
| [cpp20_lambdas.cpp](cpp20_lambdas.cpp) | c++20 | GCC >= 14 | covered | C++20 added template parameter lists on lambdas, default-construction, and capture of *this by value. |
| [cpp20_modules_basic.cpp](cpp20_modules_basic.cpp) | c++20 | GCC >= 16 | covered | C++20 modules compile an interface before an importer; this two-file fixture proves export, import, link, and execution with GCC's modules implementation. |
| [cpp20_spaceship.cpp](cpp20_spaceship.cpp) | c++20 | GCC >= 14 | covered | operator<=> ("spaceship") generates the full set of relational operators from one declaration. |
| [cpp20_using_enum.cpp](cpp20_using_enum.cpp) | c++20 | GCC >= 13 | covered | 'using enum' brings a scoped enum's enumerators into the current scope -- switch cases read 'red' instead of 'Color::red' without giving up enum class type safety. |

## lifetimes

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_ranges_borrowed_range.cpp](cpp20_ranges_borrowed_range.cpp) | c++20 | GCC >= 14 | covered | borrowed_range explains why an algorithm result from a temporary can be an iterator for span but std::ranges::dangling for an owning vector. |
| [cpp20_span.cpp](cpp20_span.cpp) | c++20 | GCC >= 14 | covered | std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind. |

## modules

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_modules_basic.cpp](cpp20_modules_basic.cpp) | c++20 | GCC >= 16 | covered | C++20 modules compile an interface before an importer; this two-file fixture proves export, import, link, and execution with GCC's modules implementation. |

## ranges

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_ranges_algorithms.cpp](cpp20_ranges_algorithms.cpp) | c++20 | GCC >= 14 | covered | std::ranges versions of classic algorithms accept full ranges + projections directly. |
| [cpp20_ranges_borrowed_range.cpp](cpp20_ranges_borrowed_range.cpp) | c++20 | GCC >= 14 | covered | borrowed_range explains why an algorithm result from a temporary can be an iterator for span but std::ranges::dangling for an owning vector. |
| [cpp20_ranges_views.cpp](cpp20_ranges_views.cpp) | c++20 | GCC >= 14 | covered | Lazy view composition with views::filter, views::transform and views::take using the | pipe. |
| [cpp20_views_keys_values.cpp](cpp20_views_keys_values.cpp) | c++20 | GCC >= 13 | covered | views::keys / views::values / views::elements project tuple-like ranges to a single component. |
| [cpp20_views_split.cpp](cpp20_views_split.cpp) | c++20 | GCC >= 13 | covered | views::split lazily divides a range on a delimiter (single element or a whole subrange) into subranges -- tokenise a string_view without allocating a token vector up front. |

## stl

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_bind_front.cpp](cpp20_bind_front.cpp) | c++20 | GCC >= 13 | covered | std::bind_front(f, args...) binds the FIRST arguments of a callable -- the modern replacement for std::bind's placeholder dance, ideal for member-function callbacks. |
| [cpp20_bit_ops.cpp](cpp20_bit_ops.cpp) | c++20 | GCC >= 14 | covered | <bit> standardises common bit tricks: popcount, countl_zero, has_single_bit, bit_ceil, bit_cast. |
| [cpp20_cmp_utilities.cpp](cpp20_cmp_utilities.cpp) | c++20 | GCC >= 13 | covered | std::cmp_less / cmp_equal / in_range compare signed and unsigned integers mathematically -- with plain operator<, -1 converts to a huge unsigned value and the comparison silently lies. |
| [cpp20_endian.cpp](cpp20_endian.cpp) | c++20 | GCC >= 13 | covered | std::endian exposes the host byte order at compile time -- portable replacement for compiler-specific macros. |
| [cpp20_erase_if.cpp](cpp20_erase_if.cpp) | c++20 | GCC >= 14 | covered | std::erase / std::erase_if removes the erase-remove idiom for every standard container. |
| [cpp20_lerp_midpoint.cpp](cpp20_lerp_midpoint.cpp) | c++20 | GCC >= 14 | covered | std::lerp and std::midpoint give numerically-careful linear interpolation and midpoint, including for integers. |
| [cpp20_map_contains.cpp](cpp20_map_contains.cpp) | c++20 | GCC >= 14 | covered | Associative containers gained .contains(key) -- clearer than 'count(k) > 0' or 'find(k) != end()'. |
| [cpp20_numbers.cpp](cpp20_numbers.cpp) | c++20 | GCC >= 14 | covered | <numbers> ships precise math constants typed as templates; no more M_PI macros. |
| [cpp20_source_location.cpp](cpp20_source_location.cpp) | c++20 | GCC >= 14 | covered | std::source_location replaces __FILE__/__LINE__/__func__ macros with a real value type. |
| [cpp20_span.cpp](cpp20_span.cpp) | c++20 | GCC >= 14 | covered | std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind. |
| [cpp20_ssize.cpp](cpp20_ssize.cpp) | c++20 | GCC >= 14 | covered | std::ssize returns a SIGNED size, eliminating the int/size_t mixed-comparison warnings of c.size(). |
| [cpp20_string_starts_ends.cpp](cpp20_string_starts_ends.cpp) | c++20 | GCC >= 14 | covered | std::string and std::string_view gained starts_with / ends_with member functions. |
| [cpp20_to_address.cpp](cpp20_to_address.cpp) | c++20 | GCC >= 13 | covered | std::to_address turns any 'fancy pointer' (smart, iterator) into a raw pointer without dereferencing. |

## templates

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_abbreviated_templates.cpp](cpp20_abbreviated_templates.cpp) | c++20 | GCC >= 13 | covered | Abbreviated function templates: 'auto' in a function parameter list declares a template parameter -- 'void f(auto x)' equals 'template<class T> void f(T x)', and a concept can constrain it in place. |
| [cpp20_concepts_constraint_error.cpp](cpp20_concepts_constraint_error.cpp) | c++20 | GCC >= 14 | negative | A constrained overload rejects a non-integral argument at selection time and produces a constraints-not-satisfied diagnostic. |
| [cpp20_concepts_intro.cpp](cpp20_concepts_intro.cpp) | c++20 | GCC >= 14 | covered | Defining and using concepts to constrain templates -- replaces SFINAE for most cases. |
| [cpp20_ctad_aggregates.cpp](cpp20_ctad_aggregates.cpp) | c++20 | GCC >= 14 | covered | Class template argument deduction now works on aggregates without writing deduction guides by hand. |
| [cpp20_ctad_alias.cpp](cpp20_ctad_alias.cpp) | c++20 | GCC >= 14 | covered | C++20 lets CTAD work through alias templates -- no manual deduction guide needed. |
| [cpp20_explicit_bool.cpp](cpp20_explicit_bool.cpp) | c++20 | GCC >= 14 | covered | explicit(bool) makes a constructor's explicitness depend on a constexpr predicate. |
| [cpp20_nttp_class.cpp](cpp20_nttp_class.cpp) | c++20 | GCC >= 14 | covered | Class types can now be non-type template parameters (NTTPs); enables compile-time string templates. |
| [cpp20_type_identity.cpp](cpp20_type_identity.cpp) | c++20 | GCC >= 13 | covered | C++20 added std::type_identity<T> (block template argument deduction in non-deduced contexts) and std::remove_cvref<T> (the common 'strip cv + reference, but not array-to-pointer decay' transformation). |

## threading

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_atomic_flag_test.cpp](cpp20_atomic_flag_test.cpp) | c++20 | GCC >= 14 | covered | C++20 added test()/wait()/notify_one to std::atomic_flag, finally making it useful beyond test_and_set. |
| [cpp20_atomic_ref.cpp](cpp20_atomic_ref.cpp) | c++20 | GCC >= 14 | covered | std::atomic_ref overlays atomic semantics on a non-atomic object you don't own. |
| [cpp20_atomic_shared_ptr.cpp](cpp20_atomic_shared_ptr.cpp) | c++20 | GCC >= 14 | covered | std::atomic<std::shared_ptr<T>> -- lock-free-style atomic load/store/exchange of shared ownership. |
| [cpp20_atomic_wait.cpp](cpp20_atomic_wait.cpp) | c++20 | GCC >= 14 | covered | std::atomic<T>::wait/notify_one/notify_all enable lock-free condition-variable-style blocking. |
| [cpp20_barrier.cpp](cpp20_barrier.cpp) | c++20 | GCC >= 14 | covered | std::barrier is reusable across phases and runs an optional completion function between phases. |
| [cpp20_binary_semaphore.cpp](cpp20_binary_semaphore.cpp) | c++20 | GCC >= 14 | covered | std::binary_semaphore = counting_semaphore<1>; classic single-slot signalling primitive. |
| [cpp20_counting_semaphore.cpp](cpp20_counting_semaphore.cpp) | c++20 | GCC >= 14 | covered | std::counting_semaphore caps concurrent access to a fixed number of "permits". |
| [cpp20_jthread.cpp](cpp20_jthread.cpp) | c++20 | GCC >= 14 | covered | std::jthread auto-joins on destruction and supports cooperative cancellation via stop_token. |
| [cpp20_latch.cpp](cpp20_latch.cpp) | c++20 | GCC >= 14 | covered | std::latch is a one-shot countdown synchroniser; threads wait until it hits zero. |
| [cpp20_stop_token.cpp](cpp20_stop_token.cpp) | c++20 | GCC >= 14 | covered | std::stop_source / stop_token / stop_callback decouple cancellation request from the threads observing it. |
| [cpp20_syncstream.cpp](cpp20_syncstream.cpp) | c++20 | GCC >= 14 | covered | std::osyncstream interleaves output safely from multiple threads -- each scope's text is emitted as one block. |

## views

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [cpp20_ranges_views.cpp](cpp20_ranges_views.cpp) | c++20 | GCC >= 14 | covered | Lazy view composition with views::filter, views::transform and views::take using the | pipe. |
| [cpp20_span.cpp](cpp20_span.cpp) | c++20 | GCC >= 14 | covered | std::span is a non-owning view over a contiguous sequence; cheap, type-erased over container kind. |
