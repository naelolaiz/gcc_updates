# TSan exits 66 when a race is detected.
gcc_feature_test(gccext_tsan_data_race  STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER thread
                 WILL_FAIL  EXPECT_OUTPUT "data race")
