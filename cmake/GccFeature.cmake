# cmake/GccFeature.cmake
# Each example is registered with one gcc_feature_test() call in its folder's
# CMakeLists.txt.

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
        set(_libstdcxx_release "")
    endif()
    set(LIBSTDCXX_RELEASE "${_libstdcxx_release}" CACHE STRING "_GLIBCXX_RELEASE" FORCE)
endif()
if(LIBSTDCXX_RELEASE)
    message(STATUS "libstdc++ release: ${LIBSTDCXX_RELEASE}")
else()
    message(STATUS "libstdc++ release: unknown")
endif()

set(_gcc_feature_sanitizers "")
if(GCC_FEATURE_SANITIZE)
    string(REPLACE "," ";" _gcc_feature_sanitizers "${GCC_FEATURE_SANITIZE}")
    message(STATUS "Active sanitizers: ${_gcc_feature_sanitizers}")
endif()

set(_gcc_feature_default_flags -Wall -Wextra -Wpedantic -O2 -pthread)
if(NOT "$ENV{NO_COLOR}" STREQUAL "")
    # Leave diagnostics colour off.
elseif(NOT "$ENV{FORCE_COLOR}" STREQUAL "" OR "$ENV{GITHUB_ACTIONS}" STREQUAL "true")
    list(APPEND _gcc_feature_default_flags -fdiagnostics-color=always)
endif()

function(_gcc_feature_remember_source SOURCE)
    get_filename_component(SOURCE "${SOURCE}" ABSOLUTE)
    set_property(GLOBAL APPEND PROPERTY GCC_FEATURE_REGISTERED_SOURCES "${SOURCE}")
endfunction()

function(_gcc_feature_has_any OUT LEFT RIGHT)
    set(_found FALSE)
    foreach(_item IN LISTS ${LEFT})
        if(_item IN_LIST ${RIGHT})
            set(_found TRUE)
            break()
        endif()
    endforeach()
    set(${OUT} "${_found}" PARENT_SCOPE)
endfunction()

function(gcc_feature_test NAME)
    cmake_parse_arguments(
        ARG
        "EXPERIMENTAL;REQUIRES_ANALYZER;WILL_FAIL"
        "STD;MIN_GCC;MAX_GCC;MIN_LIBSTDCXX;MAX_LIBSTDCXX;TOPIC;EXPECT_ERROR;EXPECT_OUTPUT"
        "EXTRA_LIBS;EXTRA_COMPILE_FLAGS;REQUIRES_SANITIZER;SKIP_SANITIZER"
        ${ARGN}
    )

    if(NOT ARG_STD OR NOT ARG_MIN_GCC OR NOT ARG_TOPIC)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): STD, MIN_GCC, and TOPIC are required")
    endif()
    if(ARG_WILL_FAIL AND NOT ARG_EXPECT_OUTPUT)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): WILL_FAIL requires EXPECT_OUTPUT <regex>")
    endif()
    if(ARG_EXPECT_OUTPUT AND NOT ARG_WILL_FAIL)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): EXPECT_OUTPUT requires WILL_FAIL")
    endif()
    if(ARG_EXPERIMENTAL AND NOT ARG_EXPECT_ERROR)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): EXPERIMENTAL requires EXPECT_ERROR <regex>")
    endif()
    if(ARG_EXPECT_ERROR AND NOT ARG_EXPERIMENTAL)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): EXPECT_ERROR requires EXPERIMENTAL")
    endif()

    set(_source "${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.cpp")
    if(NOT EXISTS "${_source}")
        message(FATAL_ERROR "gcc_feature_test(${NAME}): source file not found")
    endif()
    _gcc_feature_remember_source("${_source}")

    if(GCC_MAJOR LESS "${ARG_MIN_GCC}")
        return()
    endif()
    if(ARG_MAX_GCC AND GCC_MAJOR GREATER "${ARG_MAX_GCC}")
        return()
    endif()
    if(ARG_MIN_LIBSTDCXX AND LIBSTDCXX_RELEASE AND LIBSTDCXX_RELEASE LESS "${ARG_MIN_LIBSTDCXX}")
        return()
    endif()
    if(ARG_MAX_LIBSTDCXX AND LIBSTDCXX_RELEASE AND LIBSTDCXX_RELEASE GREATER "${ARG_MAX_LIBSTDCXX}")
        return()
    endif()

    if(GCC_FEATURE_ANALYZER)
        if(NOT ARG_REQUIRES_ANALYZER)
            return()
        endif()
    elseif(ARG_REQUIRES_ANALYZER)
        return()
    endif()

    if(ARG_REQUIRES_SANITIZER)
        _gcc_feature_has_any(_has_required ARG_REQUIRES_SANITIZER _gcc_feature_sanitizers)
        if(NOT _has_required)
            return()
        endif()
    endif()
    if(ARG_SKIP_SANITIZER)
        _gcc_feature_has_any(_has_skipped ARG_SKIP_SANITIZER _gcc_feature_sanitizers)
        if(_has_skipped)
            return()
        endif()
    endif()

    set(_compile_flags -std=${ARG_STD} ${_gcc_feature_default_flags} ${ARG_EXTRA_COMPILE_FLAGS})
    set(_link_flags "")

    if("-fopenmp" IN_LIST ARG_EXTRA_COMPILE_FLAGS)
        list(APPEND _link_flags -fopenmp)
    endif()

    if(_gcc_feature_sanitizers)
        list(REMOVE_ITEM _compile_flags -O2)
        list(APPEND _compile_flags -O1 -fno-omit-frame-pointer -g)
        foreach(_san IN LISTS _gcc_feature_sanitizers)
            list(APPEND _compile_flags -fsanitize=${_san})
            list(APPEND _link_flags -fsanitize=${_san})
        endforeach()
    endif()

    if(GCC_FEATURE_ANALYZER)
        list(APPEND _compile_flags -fanalyzer)
    endif()

    set(_labels "${ARG_TOPIC}")
    if(ARG_EXPERIMENTAL)
        list(APPEND _labels experimental)
    endif()
    set(_expect_failure_runner "${CMAKE_SOURCE_DIR}/cmake/expect_failure.cmake")

    if(ARG_EXPECT_ERROR)
        list(REMOVE_ITEM _compile_flags -fdiagnostics-color=always)
        list(APPEND _compile_flags -fdiagnostics-color=never)
        set(_command
            "${CMAKE_CXX_COMPILER}" ${_compile_flags}
            "-I${CMAKE_SOURCE_DIR}/features"
            -c "${_source}"
            -o "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.experimental.o"
        )
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_FAILURE_COMMAND=${_command}"
                    "-DEXPECT_FAILURE_REGEX=${ARG_EXPECT_ERROR}"
                    "-DEXPECT_FAILURE_KIND=compile"
                    -P "${_expect_failure_runner}"
        )
        set_tests_properties(${NAME} PROPERTIES LABELS "${_labels}")
        return()
    endif()

    add_executable(${NAME} "${_source}")
    target_compile_options(${NAME} PRIVATE ${_compile_flags})
    target_link_options(${NAME} PRIVATE ${_link_flags})
    target_include_directories(${NAME} PRIVATE "${CMAKE_SOURCE_DIR}/features")
    if(ARG_EXTRA_LIBS)
        target_link_libraries(${NAME} PRIVATE ${ARG_EXTRA_LIBS})
    endif()

    if(GCC_FEATURE_ANALYZER)
        add_test(NAME ${NAME} COMMAND "${CMAKE_COMMAND}" -E true)
    elseif(ARG_WILL_FAIL)
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_FAILURE_COMMAND=$<TARGET_FILE:${NAME}>"
                    "-DEXPECT_FAILURE_REGEX=${ARG_EXPECT_OUTPUT}"
                    "-DEXPECT_FAILURE_KIND=runtime"
                    "-DEXPECT_FAILURE_WORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
                    -P "${_expect_failure_runner}"
        )
    else()
        add_test(NAME ${NAME} COMMAND "$<TARGET_FILE:${NAME}>")
    endif()
    set_tests_properties(${NAME} PROPERTIES LABELS "${_labels}")
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
            "Missing gcc_feature_test() registrations:\n${_missing_text}"
        )
    endif()
endfunction()
