cmake_minimum_required(VERSION 3.20)
project(gcc_updates LANGUAGES CXX)

enable_testing()

include(cmake/GccFeature.cmake)

# Each leaf is a CMakeLists.txt.in template. gcc_feature_add_leaf() configures
# it into ${CMAKE_BINARY_DIR}/<rel>/CMakeLists.txt -- the resolved file is
# plain CMake with every gcc flag inline -- and then includes that copy.
# CI uploads build/features/**/CMakeLists.txt as an artifact so a reader can
# inspect the exact build configuration without parsing helpers.

# C++ language extras (gcc-features)
gcc_feature_add_leaf(features/gcc/gcc13)
gcc_feature_add_leaf(features/gcc/gcc14)
gcc_feature_add_leaf(features/gcc/gcc15)
gcc_feature_add_leaf(features/gcc/gcc16)

# GCC-specific extensions
gcc_feature_add_leaf(features/gccext/analyzer)
gcc_feature_add_leaf(features/gccext/attributes)
gcc_feature_add_leaf(features/gccext/builtins)
gcc_feature_add_leaf(features/gccext/codegen)
gcc_feature_add_leaf(features/gccext/openmp)
gcc_feature_add_leaf(features/gccext/pragmas)
gcc_feature_add_leaf(features/gccext/sanitize/asan)
gcc_feature_add_leaf(features/gccext/sanitize/integration)
gcc_feature_add_leaf(features/gccext/sanitize/leak)
gcc_feature_add_leaf(features/gccext/sanitize/tsan)
gcc_feature_add_leaf(features/gccext/sanitize/ubsan)

# Standard C++ revisions
gcc_feature_add_leaf(features/std/cpp11)
gcc_feature_add_leaf(features/std/cpp17)
gcc_feature_add_leaf(features/std/cpp20)
gcc_feature_add_leaf(features/std/cpp23)
gcc_feature_add_leaf(features/std/cpp26)

gcc_feature_validate_registered_sources()
