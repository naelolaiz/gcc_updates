# cmake/GccFeature.cmake
#
# Each features/<area>/CMakeLists.txt.in is a plain-CMake template. The root
# CMakeLists.txt configure_file()s each one into the build dir (substituting
# @VAR@ placeholders below), then include()s the resolved copy. The build-dir
# copy is therefore a self-describing CMakeLists.txt the user can read to see
# the exact gcc invocation -- no helper-parsing required.
#
# This file exposes:
#   * GCC probes:        GCC_MAJOR, LIBSTDCXX_RELEASE
#   * Sanitizer mode:    GCC_FEATURE_ACTIVE_SANITIZERS (list)
#   * Compile/link flag lists + their space-joined "_TEXT" variants for
#     @VAR@ substitution into leaf .in files.
#   * Three slim test-running helpers used by the .in files:
#       gcc_feature_normal_test(NAME)
#       gcc_feature_will_fail_test(NAME REGEX)
#       gcc_feature_expect_compile_error(NAME SOURCE STD <std> REGEX <regex>
#                                        [EXTRA_FLAGS <flags>...])
#   * gcc_feature_register(SOURCE) + gcc_feature_validate_registered_sources()
#     for orphan-source detection.

cmake_minimum_required(VERSION 3.20)

option(GCC_FEATURE_ANALYZER "Build only REQUIRES_ANALYZER examples with -fanalyzer" OFF)
set(GCC_FEATURE_SANITIZE "" CACHE STRING "Comma-separated sanitizers, e.g. address,undefined")

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This project requires GCC (got ${CMAKE_CXX_COMPILER_ID}).")
endif()

string(REGEX MATCH "^([0-9]+)" _ "${CMAKE_CXX_COMPILER_VERSION}")
set(GCC_MAJOR "${CMAKE_MATCH_1}")
message(STATUS "GCC major version: ${GCC_MAJOR}")

if(NOT DEFINED LIBSTDCXX_RELEASE)
    file(WRITE "${CMAKE_BINARY_DIR}/libstdcxx_probe.cpp" "#include <version>\n")
    execute_process(
        COMMAND "${CMAKE_CXX_COMPILER}" -x c++ -E -dM
                "${CMAKE_BINARY_DIR}/libstdcxx_probe.cpp"
        OUTPUT_VARIABLE _probe_out
        ERROR_QUIET
    )
    string(REGEX MATCH "#define _GLIBCXX_RELEASE ([0-9]+)" _ "${_probe_out}")
    if(CMAKE_MATCH_1)
        set(_libstdcxx_release "${CMAKE_MATCH_1}")
    else()
        # Probe failed; assume a high number so MIN_LIBSTDCXX gates don't skip
        # the example. Mirrors the pre-refactor behaviour.
        set(_libstdcxx_release 999)
    endif()
    set(LIBSTDCXX_RELEASE "${_libstdcxx_release}" CACHE STRING "_GLIBCXX_RELEASE" FORCE)
endif()
if(LIBSTDCXX_RELEASE EQUAL 999)
    message(STATUS "libstdc++ release: unknown (treated as newest)")
else()
    message(STATUS "libstdc++ release: ${LIBSTDCXX_RELEASE}")
endif()

set(GCC_FEATURE_ACTIVE_SANITIZERS "")
if(GCC_FEATURE_SANITIZE)
    string(REPLACE "," ";" GCC_FEATURE_ACTIVE_SANITIZERS "${GCC_FEATURE_SANITIZE}")
    message(STATUS "Active sanitizers: ${GCC_FEATURE_ACTIVE_SANITIZERS}")
endif()

# ---- Default compile/link flag lists ----

set(GCC_FEATURE_DEFAULT_COMPILE_FLAGS -Wall -Wextra -Wpedantic -O2 -pthread)
if(NOT "$ENV{NO_COLOR}" STREQUAL "")
    # Leave diagnostics colour off.
elseif(NOT "$ENV{FORCE_COLOR}" STREQUAL "" OR "$ENV{GITHUB_ACTIONS}" STREQUAL "true")
    list(APPEND GCC_FEATURE_DEFAULT_COMPILE_FLAGS -fdiagnostics-color=always)
endif()

set(GCC_FEATURE_SAN_COMPILE_FLAGS "")
set(GCC_FEATURE_SAN_LINK_FLAGS "")
if(GCC_FEATURE_ACTIVE_SANITIZERS)
    list(REMOVE_ITEM GCC_FEATURE_DEFAULT_COMPILE_FLAGS -O2)
    list(APPEND GCC_FEATURE_DEFAULT_COMPILE_FLAGS -O1 -fno-omit-frame-pointer -g)
    foreach(_san IN LISTS GCC_FEATURE_ACTIVE_SANITIZERS)
        list(APPEND GCC_FEATURE_SAN_COMPILE_FLAGS -fsanitize=${_san})
        list(APPEND GCC_FEATURE_SAN_LINK_FLAGS -fsanitize=${_san})
    endforeach()
endif()

set(GCC_FEATURE_ANALYZER_COMPILE_FLAGS "")
if(GCC_FEATURE_ANALYZER)
    set(GCC_FEATURE_ANALYZER_COMPILE_FLAGS -fanalyzer)
endif()

# Space-joined text variants for @VAR@ substitution into leaf .in files.
string(JOIN " " GCC_FEATURE_DEFAULT_COMPILE_TEXT ${GCC_FEATURE_DEFAULT_COMPILE_FLAGS})
string(JOIN " " GCC_FEATURE_SAN_COMPILE_TEXT ${GCC_FEATURE_SAN_COMPILE_FLAGS})
string(JOIN " " GCC_FEATURE_SAN_LINK_TEXT ${GCC_FEATURE_SAN_LINK_FLAGS})
string(JOIN " " GCC_FEATURE_ANALYZER_COMPILE_TEXT ${GCC_FEATURE_ANALYZER_COMPILE_FLAGS})

# ---- Test-running helpers (everything build-wise lives in the leaves) ----

function(gcc_feature_normal_test NAME)
    if(GCC_FEATURE_ANALYZER)
        # Analyzer mode: the warning fires at compile time; the binary needn't run.
        add_test(NAME ${NAME} COMMAND ${CMAKE_COMMAND} -E true)
    else()
        add_test(NAME ${NAME} COMMAND $<TARGET_FILE:${NAME}>)
    endif()
endfunction()

function(gcc_feature_will_fail_test NAME REGEX)
    add_test(
        NAME ${NAME}
        COMMAND ${CMAKE_COMMAND}
                "-DEXPECT_FAILURE_COMMAND=$<TARGET_FILE:${NAME}>"
                "-DEXPECT_FAILURE_REGEX=${REGEX}"
                "-DEXPECT_FAILURE_KIND=runtime"
                "-DEXPECT_FAILURE_WORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
                -P "${CMAKE_SOURCE_DIR}/cmake/expect_failure.cmake"
    )
endfunction()

function(gcc_feature_expect_compile_error NAME SOURCE)
    cmake_parse_arguments(_GFE "" "REGEX;STD" "EXTRA_FLAGS" ${ARGN})
    set(_flags -std=${_GFE_STD} ${GCC_FEATURE_DEFAULT_COMPILE_FLAGS} ${_GFE_EXTRA_FLAGS})
    list(REMOVE_ITEM _flags -fdiagnostics-color=always)
    list(APPEND _flags -fdiagnostics-color=never)
    set(_command
        "${CMAKE_CXX_COMPILER}" ${_flags}
        "-I${CMAKE_SOURCE_DIR}/features"
        -c "${SOURCE}"
        -o "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.experimental.o"
    )
    add_test(
        NAME ${NAME}
        COMMAND ${CMAKE_COMMAND}
                "-DEXPECT_FAILURE_COMMAND=${_command}"
                "-DEXPECT_FAILURE_REGEX=${_GFE_REGEX}"
                "-DEXPECT_FAILURE_KIND=compile"
                -P "${CMAKE_SOURCE_DIR}/cmake/expect_failure.cmake"
    )
endfunction()

# ---- Source registration / orphan detection ----

function(gcc_feature_register SOURCE)
    set_property(GLOBAL APPEND PROPERTY GCC_FEATURE_REGISTERED_SOURCES "${SOURCE}")
endfunction()

function(gcc_feature_validate_registered_sources)
    file(GLOB_RECURSE _sources CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/features/*.cpp")
    get_property(_registered GLOBAL PROPERTY GCC_FEATURE_REGISTERED_SOURCES)

    set(_missing "")
    foreach(_source IN LISTS _sources)
        list(FIND _registered "${_source}" _index)
        if(_index EQUAL -1)
            file(RELATIVE_PATH _rel "${CMAKE_SOURCE_DIR}" "${_source}")
            list(APPEND _missing "  - ${_rel}")
        endif()
    endforeach()

    if(_missing)
        list(JOIN _missing "\n" _missing_text)
        message(FATAL_ERROR
            "Missing gcc_feature_register() calls in CMakeLists.txt.in files:\n${_missing_text}"
        )
    endif()
endfunction()

# ---- Helper: configure + include a leaf's CMakeLists.txt.in ----

function(gcc_feature_add_leaf RELATIVE_PATH)
    configure_file(
        "${CMAKE_SOURCE_DIR}/${RELATIVE_PATH}/CMakeLists.txt.in"
        "${CMAKE_BINARY_DIR}/${RELATIVE_PATH}/CMakeLists.txt"
        @ONLY
    )
    include("${CMAKE_BINARY_DIR}/${RELATIVE_PATH}/CMakeLists.txt")
endfunction()
