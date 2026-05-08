# cpp26_contracts_basic is experimental: CTest compiles it directly and expects
# the compiler diagnostic below.
gcc_feature_test(cpp26_contracts_basic   STD c++26  MIN_GCC 15  TOPIC language
                 EXPERIMENTAL
                 EXTRA_COMPILE_FLAGS -fcontracts
                 EXPECT_ERROR "(expected initializer before|value parameter used in a postcondition must be const)")

gcc_feature_test(cpp26_reflection_basic  STD c++26  MIN_GCC 16  TOPIC templates
                 EXTRA_COMPILE_FLAGS -freflection)

gcc_feature_test(cpp26_saturation_arith  STD c++26  MIN_GCC 14  TOPIC stl)
