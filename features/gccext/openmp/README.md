# GCC extensions: OpenMP

_Folder: `features/gccext/openmp/`. 1 topic(s). Generated from `gcc_feature_test()` metadata and each file's `// description:` line; regenerate with `./scripts/container-dev.sh <ver> readme`._

## Topics

- [gcc-openmp](#gcc-openmp)

## gcc-openmp

| File | std | availability | status | Description |
| ---- | --- | ------------ | ------ | ----------- |
| [gccext_openmp_parallel_for.cpp](gccext_openmp_parallel_for.cpp) | c++17 | GCC >= 13 | covered | OpenMP #pragma omp parallel for parallelises a loop across threads via libgomp; -fopenmp both compiles and links. |
