# `features/std/` — C++ standard-library and language features

One subfolder per standard. Each subfolder has its own `README.md` index
listing examples grouped by topic.

| Folder | Coverage | When you'd read this |
|--------|----------|----------------------|
| [`cpp11/`](cpp11/) | The foundation: auto, decltype, lambdas, move semantics, smart pointers, std::thread + sync primitives, std::function/bind, variadic templates, etc. | If you only know pre-C++11 — start here. |
| [`cpp14/`](cpp14/) | Smaller delta, focused on what C++14 *added* on top of C++11 (e.g. std::integer_sequence). | Spot-fills between C++11 and C++17. |
| [`cpp17/`](cpp17/) | std::optional, variant, any, string_view, filesystem, structured bindings, if constexpr, fold expressions, parallel STL, attributes, byte. | Quality-of-life features layered on C++11/14. |
| [`cpp20/`](cpp20/) | Concepts, ranges/views, coroutines, the full `<thread>` upgrade (jthread, semaphores, latch, barrier, syncstream), format, span, bit ops, etc. | The biggest single jump in modern C++. |
| [`cpp23/`](cpp23/) | std::expected, mdspan, print, generator, stacktrace, flat_map; deducing this; lots of new ranges/views; library polish. | Smoothing C++20's edges plus headline items. |
| [`cpp26/`](cpp26/) | Saturation arithmetic, contracts, static reflection; some entries are experimental or GCC-version gated. | Tracking what GCC 14+ has of C++26 already. |
