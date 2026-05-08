# Need -DGCC_FEATURE_SANITIZE=address. Each binary aborts via the sanitizer.
gcc_feature_test(gccext_asan_double_free            STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER address  WILL_FAIL  EXPECT_OUTPUT "double-free")
gcc_feature_test(gccext_asan_heap_buffer_overflow   STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER address  WILL_FAIL  EXPECT_OUTPUT "heap-buffer-overflow")
gcc_feature_test(gccext_asan_heap_use_after_free    STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER address  WILL_FAIL  EXPECT_OUTPUT "heap-use-after-free")
gcc_feature_test(gccext_asan_stack_buffer_overflow  STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER address  WILL_FAIL  EXPECT_OUTPUT "stack-buffer-overflow")
