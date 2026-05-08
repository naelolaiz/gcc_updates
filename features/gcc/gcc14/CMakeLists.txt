# No EXTRA_LIBS: GCC 14 was the first release shipping std::ranges::to in the main
# libstdc++ -- no -lstdc++exp dance like the cpp23/gcc14-only std::print example.
gcc_feature_test(gcc14_libstdcxx_ranges_to  STD c++23  MIN_GCC 14  TOPIC gcc-release)
