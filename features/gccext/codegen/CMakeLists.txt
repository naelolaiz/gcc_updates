# GCC_ONLY: -fopt-info-vec-optimized and the "loop vectorized" report format
# are GCC's; clang's equivalent is -Rpass=loop-vectorize with its own wording.
gcc_feature_test(gccext_autovectorize  STD c++17  MIN_GCC 13  TOPIC gcc-codegen
                 TAGS performance simd  GCC_ONLY
                 EXTRA_COMPILE_FLAGS -O3 -fopt-info-vec-optimized
                 SKIP_SANITIZER address undefined thread
                 EXPECT_COMPILE_OUTPUT "loop vectorized")
gcc_feature_test(gccext_inline_asm     STD c++17  MIN_GCC 13  TOPIC gcc-codegen
                 TAGS architecture inline-assembly  ARCH x86
                 ALLOW_WARNINGS)
