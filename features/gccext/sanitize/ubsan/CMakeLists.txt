gcc_feature_test(gccext_ubsan_null_deref        STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: load of null pointer")
gcc_feature_test(gccext_ubsan_divide_by_zero    STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 TAGS numerics diagnostics
                 REQUIRES_SANITIZER undefined
                 EXTRA_COMPILE_FLAGS -fno-sanitize-recover=undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: division by zero")
# vptr is named in the no-recover list because clang keeps it outside its
# undefined group (gcc_feature_test() enables the check there explicitly).
gcc_feature_test(gccext_ubsan_invalid_vptr      STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 TAGS object-model diagnostics
                 REQUIRES_SANITIZER undefined
                 EXTRA_COMPILE_FLAGS -fno-sanitize-recover=undefined,vptr
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: member call on address")
gcc_feature_test(gccext_ubsan_misaligned_access STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 TAGS memory-safety diagnostics
                 REQUIRES_SANITIZER undefined
                 EXTRA_COMPILE_FLAGS -fno-sanitize-recover=undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: load of misaligned address")
gcc_feature_test(gccext_ubsan_shift             STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: shift exponent")
gcc_feature_test(gccext_ubsan_signed_overflow   STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 REQUIRES_SANITIZER undefined
                 WILL_FAIL  EXPECT_OUTPUT "runtime error: signed integer overflow"
                 EXTRA_COMPILE_FLAGS -fno-sanitize-recover=undefined)
