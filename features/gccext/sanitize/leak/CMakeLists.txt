# Need -DGCC_FEATURE_SANITIZE=leak (or address — lsan ships with asan).
gcc_feature_test(gccext_lsan_simple_leak  STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER leak address
                 WILL_FAIL  EXPECT_OUTPUT "LeakSanitizer|detected memory leaks")
