# No EXTRA_LIBS / EXTRA_COMPILE_FLAGS: GCC 13 was the first release where libstdc++
# shipped a usable <format> by default -- nothing extra to wire up.
gcc_feature_test(gcc13_libstdcxx_format  STD c++20  MIN_GCC 13  TOPIC gcc-release)
