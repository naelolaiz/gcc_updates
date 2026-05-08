# No EXTRA_LIBS: GCC 15 promoted std::print/std::println into the main libstdc++.
# Compare with features/std/cpp23/CMakeLists.txt (the cpp23_print invocation), where the same feature on GCC 14 needs `EXTRA_LIBS stdc++exp`.
gcc_feature_test(gcc15_default_print  STD c++23  MIN_GCC 15  TOPIC gcc-release)
