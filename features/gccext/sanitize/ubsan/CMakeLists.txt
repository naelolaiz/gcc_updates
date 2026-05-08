gcc_feature_test(gccext_ubsan_null_deref        STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: load of null pointer")
gcc_feature_test(gccext_ubsan_shift             STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: shift exponent")
gcc_feature_test(gccext_ubsan_signed_overflow   STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: signed integer overflow"
                 EXTRA_COMPILE_FLAGS -fno-sanitize-recover=undefined)
