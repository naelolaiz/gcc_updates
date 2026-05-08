# -fopenmp is automatically forwarded to link options by gcc_feature_test().
# SKIP_SANITIZER thread: TSan + OpenMP is unsupported.
gcc_feature_test(gccext_openmp_parallel_for  STD c++17  MIN_GCC 13  TOPIC gcc-openmp
                 EXTRA_COMPILE_FLAGS -fopenmp
                 SKIP_SANITIZER thread)
