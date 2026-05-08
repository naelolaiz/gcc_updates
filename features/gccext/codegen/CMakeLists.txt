gcc_feature_test(gccext_autovectorize  STD c++17  MIN_GCC 13  TOPIC gcc-codegen
                 EXTRA_COMPILE_FLAGS -O3 -fopt-info-vec=/dev/null)
gcc_feature_test(gccext_inline_asm     STD c++17  MIN_GCC 13  TOPIC gcc-codegen)
