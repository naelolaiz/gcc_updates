# GCC extensions: OpenMP

_Folder: `features/gccext/openmp/`. 1 topic(s). Index of examples; build metadata lives in `CMakeLists.txt` next to each `.cpp`. Update by hand when adding new examples._

## gcc-openmp

| File | std | min-gcc | Description |
| ---- | --- | ------- | ----------- |
| [gccext_openmp_parallel_for.cpp](gccext_openmp_parallel_for.cpp) | c++17 | 13 | OpenMP #pragma omp parallel for parallelises a loop across threads via libgomp; -fopenmp both compiles and links. |
