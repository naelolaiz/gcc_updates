# cmake/GccFeature.cmake
# Each example is registered with one gcc_feature_test() call in its folder's
# CMakeLists.txt.

cmake_minimum_required(VERSION 3.20)

option(GCC_FEATURE_ANALYZER "Build only REQUIRES_ANALYZER examples with -fanalyzer" OFF)
set(GCC_FEATURE_SANITIZE "" CACHE STRING "Comma-separated sanitizers, e.g. address,undefined")
set(GCC_FEATURE_README "check" CACHE STRING
    "Per-bucket README index mode: check (fail configure when stale), write (regenerate), OFF")
set(GCC_FEATURE_README_OUT "${CMAKE_SOURCE_DIR}" CACHE PATH
    "Output root for GCC_FEATURE_README=write (mirrors the features/ tree)")

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

    set(_source "${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.cpp")
    if(NOT EXISTS "${_source}")
        message(FATAL_ERROR "gcc_feature_test(${NAME}): source file not found")
    endif()
    _gcc_feature_remember_source("${_source}")

    # Record index metadata BEFORE any gating below: the README indexes list
    # every example, including ones this configure mode does not register.
    file(RELATIVE_PATH _rel_dir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    set_property(GLOBAL APPEND PROPERTY GCC_FEATURE_INDEX_DIRS "${_rel_dir}")
    set_property(GLOBAL APPEND PROPERTY "GCC_FEATURE_INDEX_${_rel_dir}" "${NAME}")
    if(ARG_EXPERIMENTAL)
        set(_experimental 1)
    else()
        set(_experimental 0)
    endif()
    set_property(GLOBAL PROPERTY "GCC_FEATURE_META_${NAME}"
                 "${ARG_STD}" "${ARG_MIN_GCC}" "${ARG_TOPIC}" "${_experimental}")

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

    if(ARG_STD STREQUAL "default")
        # STD default: pass no -std flag at all -- the example deliberately
        # tests the compiler's default dialect (see features/gcc/defaults/).
        set(_compile_flags ${_gcc_feature_default_flags} ${ARG_EXTRA_COMPILE_FLAGS})
    else()
        set(_compile_flags -std=${ARG_STD} ${_gcc_feature_default_flags} ${ARG_EXTRA_COMPILE_FLAGS})
    endif()
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

# Builds each bucket's README.md index from the gcc_feature_test() metadata
# plus the '// description:' line of every .cpp. The existing README's first
# line (the H1 title) is hand-written and preserved; everything below it is
# generated. Modes (GCC_FEATURE_README): check fails the configure when an
# index on disk is stale; write regenerates them under GCC_FEATURE_README_OUT;
# OFF skips.
function(gcc_feature_readme_indexes)
    if(GCC_FEATURE_README STREQUAL "OFF")
        return()
    endif()
    if(NOT GCC_FEATURE_README MATCHES "^(check|write)$")
        message(FATAL_ERROR "GCC_FEATURE_README must be check, write, or OFF (got '${GCC_FEATURE_README}')")
    endif()

    get_property(_dirs GLOBAL PROPERTY GCC_FEATURE_INDEX_DIRS)
    list(REMOVE_DUPLICATES _dirs)
    list(SORT _dirs)

    set(_stale "")
    foreach(_dir IN LISTS _dirs)
        set(_readme "${CMAKE_SOURCE_DIR}/${_dir}/README.md")
        if(EXISTS "${_readme}")
            file(STRINGS "${_readme}" _h1 LIMIT_COUNT 1)
        else()
            set(_h1 "# `${_dir}` examples")
        endif()

        get_property(_names GLOBAL PROPERTY "GCC_FEATURE_INDEX_${_dir}")
        list(SORT _names)

        set(_topics "")
        foreach(_name IN LISTS _names)
            get_property(_meta GLOBAL PROPERTY "GCC_FEATURE_META_${_name}")
            list(GET _meta 2 _topic)
            list(APPEND _topics "${_topic}")
        endforeach()
        list(REMOVE_DUPLICATES _topics)
        list(SORT _topics)
        list(LENGTH _topics _topic_count)

        set(_content "${_h1}\n\n")
        string(APPEND _content
            "_Folder: `${_dir}/`. ${_topic_count} topic(s). Generated from"
            " `gcc_feature_test()` metadata and each file's `// description:`"
            " line; regenerate with `./scripts/podman-dev.sh <ver> readme`._\n")
        string(APPEND _content "\n## Topics\n\n")
        foreach(_topic IN LISTS _topics)
            string(APPEND _content "- [${_topic}](#${_topic})\n")
        endforeach()

        foreach(_topic IN LISTS _topics)
            string(APPEND _content "\n## ${_topic}\n\n")
            string(APPEND _content "| File | std | min-gcc | Description |\n")
            string(APPEND _content "| ---- | --- | ------- | ----------- |\n")
            foreach(_name IN LISTS _names)
                get_property(_meta GLOBAL PROPERTY "GCC_FEATURE_META_${_name}")
                list(GET _meta 0 _std)
                list(GET _meta 1 _min_gcc)
                list(GET _meta 2 _entry_topic)
                list(GET _meta 3 _experimental)
                if(NOT _entry_topic STREQUAL _topic)
                    continue()
                endif()
                file(READ "${CMAKE_SOURCE_DIR}/${_dir}/${_name}.cpp" _head LIMIT 2048)
                if(NOT _head MATCHES "// description: ([^\n]+)")
                    message(FATAL_ERROR
                        "${_dir}/${_name}.cpp: missing '// description: ...' first-line comment")
                endif()
                set(_desc "${CMAKE_MATCH_1}")
                if(_experimental)
                    string(APPEND _desc " *(experimental)*")
                endif()
                string(APPEND _content
                    "| [${_name}.cpp](${_name}.cpp) | ${_std} | ${_min_gcc} | ${_desc} |\n")
            endforeach()
        endforeach()

        if(GCC_FEATURE_README STREQUAL "write")
            file(WRITE "${GCC_FEATURE_README_OUT}/${_dir}/README.md" "${_content}")
        else()
            set(_existing "")
            if(EXISTS "${_readme}")
                file(READ "${_readme}" _existing)
            endif()
            if(NOT _existing STREQUAL _content)
                list(APPEND _stale "  - ${_dir}/README.md")
            endif()
        endif()
    endforeach()

    # Cross-bucket topic index (features/TOPICS.md): every topic label with
    # its examples from ALL buckets, e.g. 'threading' from C++11 to C++20.
    set(_all_topics "")
    foreach(_dir IN LISTS _dirs)
        get_property(_names GLOBAL PROPERTY "GCC_FEATURE_INDEX_${_dir}")
        list(SORT _names)
        string(REPLACE "features/" "" _bucket "${_dir}")
        foreach(_name IN LISTS _names)
            get_property(_meta GLOBAL PROPERTY "GCC_FEATURE_META_${_name}")
            list(GET _meta 0 _std)
            list(GET _meta 1 _min_gcc)
            list(GET _meta 2 _topic)
            if(NOT _topic IN_LIST _all_topics)
                list(APPEND _all_topics "${_topic}")
                set_property(GLOBAL PROPERTY "GCC_FEATURE_TOPIC_ROWS_${_topic}" "")
            endif()
            set_property(GLOBAL APPEND PROPERTY "GCC_FEATURE_TOPIC_ROWS_${_topic}"
                         "${_bucket}|${_name}|${_std}|${_min_gcc}")
        endforeach()
    endforeach()
    list(SORT _all_topics)
    list(LENGTH _all_topics _n_topics)

    set(_tcontent "# Topic index\n\n")
    string(APPEND _tcontent
        "_All ${_n_topics} topics across every bucket, generated from"
        " `gcc_feature_test()` metadata; regenerate with"
        " `./scripts/podman-dev.sh <ver> readme`. Topics double as CTest"
        " labels: `ctest -L threading` runs one topic everywhere._\n")
    string(APPEND _tcontent "\n## Topics\n\n")
    foreach(_topic IN LISTS _all_topics)
        string(APPEND _tcontent "- [${_topic}](#${_topic})\n")
    endforeach()
    foreach(_topic IN LISTS _all_topics)
        string(APPEND _tcontent "\n## ${_topic}\n\n")
        string(APPEND _tcontent "| Example | Bucket | std | min-gcc |\n")
        string(APPEND _tcontent "| ------- | ------ | --- | ------- |\n")
        get_property(_rows GLOBAL PROPERTY "GCC_FEATURE_TOPIC_ROWS_${_topic}")
        foreach(_row IN LISTS _rows)
            string(REPLACE "|" ";" _fields "${_row}")
            list(GET _fields 0 _bucket)
            list(GET _fields 1 _name)
            list(GET _fields 2 _std)
            list(GET _fields 3 _min_gcc)
            string(APPEND _tcontent
                "| [${_name}.cpp](${_bucket}/${_name}.cpp) | ${_bucket} | ${_std} | ${_min_gcc} |\n")
        endforeach()
    endforeach()

    if(GCC_FEATURE_README STREQUAL "write")
        file(WRITE "${GCC_FEATURE_README_OUT}/features/TOPICS.md" "${_tcontent}")
    else()
        set(_texisting "")
        if(EXISTS "${CMAKE_SOURCE_DIR}/features/TOPICS.md")
            file(READ "${CMAKE_SOURCE_DIR}/features/TOPICS.md" _texisting)
        endif()
        if(NOT _texisting STREQUAL _tcontent)
            list(APPEND _stale "  - features/TOPICS.md")
        endif()
    endif()

    if(_stale)
        list(JOIN _stale "\n" _stale_text)
        message(FATAL_ERROR
            "Stale README index(es):\n${_stale_text}\n"
            "Regenerate with ./scripts/podman-dev.sh <gcc-ver> readme "
            "(cmake -DGCC_FEATURE_README=write under the hood).")
    endif()
endfunction()
