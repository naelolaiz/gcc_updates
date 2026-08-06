cmake_minimum_required(VERSION 3.20)
project(gcc_updates LANGUAGES CXX)

enable_testing()

include(cmake/GccFeature.cmake)

# C++ language extras (gcc-features)
add_subdirectory(features/gcc/defaults)
add_subdirectory(features/gcc/gcc13)
add_subdirectory(features/gcc/gcc14)
add_subdirectory(features/gcc/gcc15)
add_subdirectory(features/gcc/gcc16)

# GCC-specific extensions
add_subdirectory(features/gccext/analyzer)
add_subdirectory(features/gccext/attributes)
add_subdirectory(features/gccext/builtins)
add_subdirectory(features/gccext/codegen)
add_subdirectory(features/gccext/openmp)
add_subdirectory(features/gccext/pragmas)
add_subdirectory(features/gccext/sanitize/asan)
add_subdirectory(features/gccext/sanitize/integration)
add_subdirectory(features/gccext/sanitize/leak)
add_subdirectory(features/gccext/sanitize/tsan)
add_subdirectory(features/gccext/sanitize/ubsan)

# Standard C++ revisions
add_subdirectory(features/std/cpp11)
add_subdirectory(features/std/cpp14)
add_subdirectory(features/std/cpp17)
add_subdirectory(features/std/cpp20)
add_subdirectory(features/std/cpp23)
add_subdirectory(features/std/cpp26)

gcc_feature_validate_registered_sources()
gcc_feature_validate_doc_links()
gcc_feature_readme_indexes()
