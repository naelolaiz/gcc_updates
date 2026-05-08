# C++26 contracts are gated behind -fcontracts (off-by-default opt-in). The feature
# is half-landed in current GCC, so we mark the example EXPERIMENTAL: CTest invokes
# the compiler directly and asserts the diagnostic below appears -- a passing test
# means GCC still rejects the syntax, which is the documented status today.
gcc_feature_test(cpp26_contracts_basic   STD c++26  MIN_GCC 15  TOPIC language
                 EXPERIMENTAL
                 EXTRA_COMPILE_FLAGS -fcontracts
                 EXPECT_ERROR "(expected initializer before|value parameter used in a postcondition must be const)")

# C++26 static reflection (P2996) is gated behind -freflection in GCC 16; without
# the flag the ^^ token / std::meta APIs aren't recognised.
gcc_feature_test(cpp26_reflection_basic  STD c++26  MIN_GCC 16  TOPIC templates
                 EXTRA_COMPILE_FLAGS -freflection)

gcc_feature_test(cpp26_saturation_arith  STD c++26  MIN_GCC 14  TOPIC stl)
