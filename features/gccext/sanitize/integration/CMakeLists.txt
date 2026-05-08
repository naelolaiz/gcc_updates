gcc_feature_test(gccext_assume_under_sanitize  STD c++23  MIN_GCC 14  TOPIC gcc-sanitize)
gcc_feature_test(gccext_glibcxx_debug          STD c++17  MIN_GCC 13  TOPIC gcc-sanitize
                 EXTRA_COMPILE_FLAGS -D_GLIBCXX_DEBUG)
gcc_feature_test(gccext_no_sanitize_attribute  STD c++17  MIN_GCC 13  TOPIC gcc-sanitize)
