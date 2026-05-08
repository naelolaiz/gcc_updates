# No EXTRA_COMPILE_FLAGS beyond -std=c++26: GCC 16 enables ~two-thirds of C++26 by
# default. Contrast cpp26_contracts_basic / cpp26_reflection_basic, which still need
# explicit -fcontracts / -freflection opt-in flags.
gcc_feature_test(gcc16_cpp26_features_default  STD c++26  MIN_GCC 16  TOPIC gcc-release)
