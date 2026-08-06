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

set(_gcc_feature_default_flags -Wall -Wextra -Wpedantic -Werror -O2 -pthread)
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
        "EXPERIMENTAL;REQUIRES_ANALYZER;WILL_FAIL;ALLOW_WARNINGS;COMPILE_ONLY"
        "STD;MIN_GCC;MAX_GCC;MIN_LIBSTDCXX;MAX_LIBSTDCXX;TOPIC;EXPECT_ERROR;EXPECT_OUTPUT;EXPECT_COMPILE_OUTPUT;EXPECT_RUN_OUTPUT;STATUS;ARCH;FEATURE_MACRO;PROPOSAL;SKIP_REASON;MODULE_INTERFACE"
        "EXTRA_LIBS;EXTRA_COMPILE_FLAGS;REQUIRES_SANITIZER;SKIP_SANITIZER;TAGS;PREREQUISITES"
        ${ARGN}
    )

    if(ARG_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR
            "gcc_feature_test(${NAME}): unknown arguments '${ARG_UNPARSED_ARGUMENTS}'")
    endif()
    if(NOT ARG_STD OR NOT ARG_MIN_GCC OR NOT ARG_TOPIC)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): STD, MIN_GCC, and TOPIC are required")
    endif()
    get_property(_known_names GLOBAL PROPERTY GCC_FEATURE_NAMES)
    if(NAME IN_LIST _known_names)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): duplicate example name")
    endif()
    set_property(GLOBAL APPEND PROPERTY GCC_FEATURE_NAMES "${NAME}")
    if(ARG_WILL_FAIL AND NOT ARG_EXPECT_OUTPUT)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): WILL_FAIL requires EXPECT_OUTPUT <regex>")
    endif()
    if(ARG_EXPECT_OUTPUT AND NOT ARG_WILL_FAIL)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): EXPECT_OUTPUT requires WILL_FAIL")
    endif()
    if(ARG_EXPERIMENTAL AND NOT ARG_EXPECT_ERROR)
        message(FATAL_ERROR "gcc_feature_test(${NAME}): EXPERIMENTAL requires EXPECT_ERROR <regex>")
    endif()
    if(ARG_REQUIRES_ANALYZER AND NOT ARG_EXPECT_COMPILE_OUTPUT)
        message(FATAL_ERROR
            "gcc_feature_test(${NAME}): REQUIRES_ANALYZER requires EXPECT_COMPILE_OUTPUT <regex>"
        )
    endif()
    if(ARG_MODULE_INTERFACE AND (ARG_EXPECT_ERROR OR ARG_REQUIRES_ANALYZER OR ARG_WILL_FAIL))
        message(FATAL_ERROR
            "gcc_feature_test(${NAME}): MODULE_INTERFACE cannot be combined with failure/analyzer modes"
        )
    endif()
    if(ARG_COMPILE_ONLY AND (ARG_EXPECT_ERROR OR ARG_REQUIRES_ANALYZER OR ARG_WILL_FAIL
                             OR ARG_MODULE_INTERFACE OR ARG_EXPECT_RUN_OUTPUT))
        message(FATAL_ERROR
            "gcc_feature_test(${NAME}): COMPILE_ONLY cannot be combined with another proof mode"
        )
    endif()
    if(ARG_COMPILE_ONLY AND NOT ARG_SKIP_REASON)
        message(FATAL_ERROR
            "gcc_feature_test(${NAME}): COMPILE_ONLY requires SKIP_REASON <text>"
        )
    endif()

    set(_source "${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.cpp")
    if(NOT EXISTS "${_source}")
        message(FATAL_ERROR "gcc_feature_test(${NAME}): source file not found")
    endif()
    _gcc_feature_remember_source("${_source}")
    if(ARG_MODULE_INTERFACE)
        set(_module_interface "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_MODULE_INTERFACE}")
        if(NOT EXISTS "${_module_interface}")
            message(FATAL_ERROR
                "gcc_feature_test(${NAME}): module interface '${ARG_MODULE_INTERFACE}' not found"
            )
        endif()
        _gcc_feature_remember_source("${_module_interface}")
    endif()

    # Record index metadata BEFORE any gating below: the README indexes list
    # every example, including ones this configure mode does not register.
    file(RELATIVE_PATH _rel_dir "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    set_property(GLOBAL APPEND PROPERTY GCC_FEATURE_INDEX_DIRS "${_rel_dir}")
    set_property(GLOBAL APPEND PROPERTY "GCC_FEATURE_INDEX_${_rel_dir}" "${NAME}")
    # Every example must carry the description and reference header lines;
    # the why/before/pitfall context comments are optional.
    file(READ "${_source}" _context_head LIMIT 4096)
    foreach(_context_field description reference)
        if(NOT _context_head MATCHES "// ${_context_field}: [^\n]+")
            message(FATAL_ERROR
                "gcc_feature_test(${NAME}): missing "
                "'// ${_context_field}: ...' header comment"
            )
        endif()
    endforeach()
    if(NOT ARG_STATUS)
        if(ARG_COMPILE_ONLY)
            set(ARG_STATUS "compile-only")
        elseif(ARG_EXPERIMENTAL)
            set(ARG_STATUS "expected-failure")
        elseif(ARG_EXPECT_ERROR)
            set(ARG_STATUS "negative")
        else()
            set(ARG_STATUS "covered")
        endif()
    endif()
    if(NOT ARG_STATUS MATCHES "^(covered|partial|negative|expected-failure|compile-only)$")
        message(FATAL_ERROR
            "gcc_feature_test(${NAME}): unsupported STATUS '${ARG_STATUS}'"
        )
    endif()
    if(NOT ARG_ARCH)
        set(ARG_ARCH "portable")
    endif()
    set(_tags "${ARG_TOPIC}")
    list(APPEND _tags ${ARG_TAGS})
    list(REMOVE_DUPLICATES _tags)
    foreach(_field STD MIN_GCC MAX_GCC MIN_LIBSTDCXX MAX_LIBSTDCXX TOPIC STATUS
                   ARCH FEATURE_MACRO PROPOSAL SKIP_REASON MODULE_INTERFACE)
        set_property(GLOBAL PROPERTY "GCC_FEATURE_META_${NAME}_${_field}" "${ARG_${_field}}")
    endforeach()
    foreach(_field TAGS PREREQUISITES EXTRA_LIBS EXTRA_COMPILE_FLAGS
                   REQUIRES_SANITIZER SKIP_SANITIZER)
        set_property(GLOBAL PROPERTY "GCC_FEATURE_META_${NAME}_${_field}" "${ARG_${_field}}")
    endforeach()
    set_property(GLOBAL PROPERTY "GCC_FEATURE_META_${NAME}_TAGS" "${_tags}")

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
    if(ARG_ALLOW_WARNINGS OR ARG_WILL_FAIL)
        # Deliberate runtime defects can also trigger a front-end warning;
        # the sanitizer report, not warning cleanliness, is their proof.
        list(REMOVE_ITEM _compile_flags -Werror)
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
        # Keep paths and allocations visible to the analyzer. At -O2 the
        # optimizer can erase a deliberate leak before path analysis runs.
        list(REMOVE_ITEM _compile_flags -O2)
        list(APPEND _compile_flags -O0 -fanalyzer)
    endif()

    set(_labels ${_tags} "std-${ARG_STD}")
    if(ARG_EXPERIMENTAL)
        list(APPEND _labels experimental)
    endif()
    set(_expect_failure_runner "${CMAKE_SOURCE_DIR}/cmake/expect_failure.cmake")
    set(_expect_command_runner "${CMAKE_SOURCE_DIR}/cmake/expect_command.cmake")

    if(ARG_MODULE_INTERFACE)
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DMODULE_CXX=${CMAKE_CXX_COMPILER}"
                    "-DMODULE_FLAGS=${_compile_flags}"
                    "-DMODULE_INCLUDE=${CMAKE_SOURCE_DIR}/features"
                    "-DMODULE_INTERFACE=${_module_interface}"
                    "-DMODULE_IMPORTER=${_source}"
                    "-DMODULE_WORK=${CMAKE_CURRENT_BINARY_DIR}/${NAME}_module"
                    -P "${CMAKE_SOURCE_DIR}/cmake/run_module_example.cmake"
        )
        set_tests_properties(${NAME} PROPERTIES LABELS "${_labels}")
        return()
    endif()

    if(ARG_EXPECT_ERROR)
        list(REMOVE_ITEM _compile_flags -fdiagnostics-color=always)
        list(APPEND _compile_flags -fdiagnostics-color=never)
        set(_command
            "${CMAKE_CXX_COMPILER}" ${_compile_flags}
            "-I${CMAKE_SOURCE_DIR}/features"
            -c "${_source}"
            -o "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.expected-error.o"
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

    if(ARG_REQUIRES_ANALYZER)
        set(_command
            "${CMAKE_CXX_COMPILER}" ${_compile_flags}
            "-I${CMAKE_SOURCE_DIR}/features"
            -c "${_source}"
            -o "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.analyzer.o"
        )
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_COMMAND=${_command}"
                    "-DEXPECT_COMMAND_REGEX=${ARG_EXPECT_COMPILE_OUTPUT}"
                    "-DEXPECT_COMMAND_EXIT=zero"
                    "-DEXPECT_COMMAND_KIND=analyzer compile"
                    -P "${_expect_command_runner}"
        )
        set_tests_properties(${NAME} PROPERTIES LABELS "${_labels}")
        return()
    endif()

    if(ARG_EXPECT_COMPILE_OUTPUT)
        set(_command
            "${CMAKE_CXX_COMPILER}" ${_compile_flags}
            "-I${CMAKE_SOURCE_DIR}/features"
            -c "${_source}"
            -o "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.diagnostic.o"
        )
        add_test(
            NAME ${NAME}_compile_diagnostic
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_COMMAND=${_command}"
                    "-DEXPECT_COMMAND_REGEX=${ARG_EXPECT_COMPILE_OUTPUT}"
                    "-DEXPECT_COMMAND_EXIT=zero"
                    "-DEXPECT_COMMAND_KIND=compile diagnostic"
                    -P "${_expect_command_runner}"
        )
        set_tests_properties(${NAME}_compile_diagnostic PROPERTIES LABELS "${_labels}")
    endif()

    if(ARG_COMPILE_ONLY)
        set(_command
            "${CMAKE_CXX_COMPILER}" ${_compile_flags}
            "-I${CMAKE_SOURCE_DIR}/features"
            -c "${_source}"
            -o "${CMAKE_CURRENT_BINARY_DIR}/${NAME}.compile-only.o"
        )
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_COMMAND=${_command}"
                    "-DEXPECT_COMMAND_EXIT=zero"
                    "-DEXPECT_COMMAND_KIND=compile-only check"
                    -P "${_expect_command_runner}"
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

    if(ARG_WILL_FAIL)
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_FAILURE_COMMAND=$<TARGET_FILE:${NAME}>"
                    "-DEXPECT_FAILURE_REGEX=${ARG_EXPECT_OUTPUT}"
                    "-DEXPECT_FAILURE_KIND=runtime"
                    "-DEXPECT_FAILURE_WORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
                    -P "${_expect_failure_runner}"
        )
    elseif(ARG_EXPECT_RUN_OUTPUT)
        add_test(
            NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                    "-DEXPECT_COMMAND=$<TARGET_FILE:${NAME}>"
                    "-DEXPECT_COMMAND_REGEX=${ARG_EXPECT_RUN_OUTPUT}"
                    "-DEXPECT_COMMAND_EXIT=zero"
                    "-DEXPECT_COMMAND_KIND=runtime"
                    "-DEXPECT_COMMAND_WORKING_DIRECTORY=${CMAKE_CURRENT_BINARY_DIR}"
                    -P "${_expect_command_runner}"
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

    # Prerequisite names are part of the public dependency graph. Catch typos
    # during configure rather than generating dead links in coverage.yml.
    get_property(_names GLOBAL PROPERTY GCC_FEATURE_NAMES)
    foreach(_name IN LISTS _names)
        _gcc_feature_meta(_prerequisites "${_name}" PREREQUISITES)
        foreach(_prerequisite IN LISTS _prerequisites)
            if(_prerequisite STREQUAL _name)
                message(FATAL_ERROR
                    "gcc_feature_test(${_name}): an example cannot require itself"
                )
            endif()
            if(NOT _prerequisite IN_LIST _names)
                message(FATAL_ERROR
                    "gcc_feature_test(${_name}): unknown prerequisite '${_prerequisite}'"
                )
            endif()
        endforeach()
    endforeach()
endfunction()

# Read one metadata field recorded by gcc_feature_test().
function(_gcc_feature_meta OUT NAME FIELD)
    get_property(_value GLOBAL PROPERTY "GCC_FEATURE_META_${NAME}_${FIELD}")
    set(${OUT} "${_value}" PARENT_SCOPE)
endfunction()

function(_gcc_feature_yaml_quote OUT VALUE)
    string(REPLACE "'" "''" _quoted "${VALUE}")
    set(${OUT} "'${_quoted}'" PARENT_SCOPE)
endfunction()

# Builds each bucket's README.md index, the cross-topic index, and the
# machine-readable root coverage.yml. Modes (GCC_FEATURE_README): check fails
# configure when generated documentation is stale; write regenerates it under
# GCC_FEATURE_README_OUT; OFF skips.
# Hand-written markdown (README.md, docs/, reference paths) can rot silently:
# a renamed or deleted example leaves dead relative links that no build step
# notices. Validate at configure time that every relative link target exists.
# External links, and in-page #anchors are out of scope (file existence only).
function(gcc_feature_validate_doc_links)
    file(GLOB _md_files
         "${CMAKE_SOURCE_DIR}/*.md"
         "${CMAKE_SOURCE_DIR}/docs/*.md")
    file(GLOB_RECURSE _md_features "${CMAKE_SOURCE_DIR}/features/*.md")
    list(APPEND _md_files ${_md_features})
    list(SORT _md_files)

    set(_broken "")
    foreach(_md IN LISTS _md_files)
        file(READ "${_md}" _text)
        get_filename_component(_md_dir "${_md}" DIRECTORY)
        # Scan "](target)" occurrences by hand: string(REGEX MATCHALL) folds
        # matches that contain parentheses into one escaped item on the CMake
        # versions the gcc:N images ship, so its result cannot be iterated.
        set(_rest "${_text}")
        while(TRUE)
            string(FIND "${_rest}" "](" _open)
            if(_open EQUAL -1)
                break()
            endif()
            math(EXPR _open "${_open} + 2")
            string(SUBSTRING "${_rest}" ${_open} -1 _rest)
            string(FIND "${_rest}" ")" _close)
            if(_close EQUAL -1)
                break()
            endif()
            string(SUBSTRING "${_rest}" 0 ${_close} _target)
            if(_target MATCHES "^(https?|mailto):" OR _target MATCHES "^#"
               OR _target MATCHES "[\n]")
                continue()
            endif()
            string(REGEX REPLACE " \"[^\"]*\"$" "" _target "${_target}")
            string(REGEX REPLACE "#.*$" "" _target "${_target}")
            if(_target STREQUAL "")
                continue()
            endif()
            if(NOT EXISTS "${_md_dir}/${_target}")
                file(RELATIVE_PATH _rel "${CMAKE_SOURCE_DIR}" "${_md}")
                list(APPEND _broken "  - ${_rel}: ${_target}")
            endif()
        endwhile()
    endforeach()

    if(_broken)
        list(REMOVE_DUPLICATES _broken)
        list(JOIN _broken "\n" _broken_text)
        message(FATAL_ERROR
            "Broken relative links in markdown docs:\n${_broken_text}")
    endif()
endfunction()

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
            _gcc_feature_meta(_tags "${_name}" TAGS)
            list(APPEND _topics ${_tags})
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
            string(APPEND _content "| File | std | availability | status | Description |\n")
            string(APPEND _content "| ---- | --- | ------------ | ------ | ----------- |\n")
            foreach(_name IN LISTS _names)
                _gcc_feature_meta(_std "${_name}" STD)
                _gcc_feature_meta(_min_gcc "${_name}" MIN_GCC)
                _gcc_feature_meta(_max_gcc "${_name}" MAX_GCC)
                _gcc_feature_meta(_min_lib "${_name}" MIN_LIBSTDCXX)
                _gcc_feature_meta(_max_lib "${_name}" MAX_LIBSTDCXX)
                _gcc_feature_meta(_entry_tags "${_name}" TAGS)
                _gcc_feature_meta(_status "${_name}" STATUS)
                if(NOT _topic IN_LIST _entry_tags)
                    continue()
                endif()
                file(READ "${CMAKE_SOURCE_DIR}/${_dir}/${_name}.cpp" _head LIMIT 2048)
                if(NOT _head MATCHES "// description: ([^\n]+)")
                    message(FATAL_ERROR
                        "${_dir}/${_name}.cpp: missing '// description: ...' first-line comment")
                endif()
                set(_desc "${CMAKE_MATCH_1}")
                set(_availability "GCC >= ${_min_gcc}")
                if(_max_gcc)
                    string(APPEND _availability ", <= ${_max_gcc}")
                endif()
                if(_min_lib)
                    string(APPEND _availability "; libstdc++ >= ${_min_lib}")
                endif()
                if(_max_lib)
                    string(APPEND _availability ", <= ${_max_lib}")
                endif()
                string(APPEND _content
                    "| [${_name}.cpp](${_name}.cpp) | ${_std} | ${_availability} | ${_status} | ${_desc} |\n")
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
            _gcc_feature_meta(_std "${_name}" STD)
            _gcc_feature_meta(_min_gcc "${_name}" MIN_GCC)
            _gcc_feature_meta(_min_lib "${_name}" MIN_LIBSTDCXX)
            _gcc_feature_meta(_tags "${_name}" TAGS)
            if(_min_lib)
                set(_topic_min_lib "${_min_lib}")
            else()
                set(_topic_min_lib "—")
            endif()
            foreach(_topic IN LISTS _tags)
                if(NOT _topic IN_LIST _all_topics)
                    list(APPEND _all_topics "${_topic}")
                    set_property(GLOBAL PROPERTY "GCC_FEATURE_TOPIC_ROWS_${_topic}" "")
                endif()
                set_property(GLOBAL APPEND PROPERTY "GCC_FEATURE_TOPIC_ROWS_${_topic}"
                             "${_bucket}|${_name}|${_std}|${_min_gcc}|${_topic_min_lib}")
            endforeach()
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
        string(APPEND _tcontent "| Example | Bucket | std | GCC | libstdc++ |\n")
        string(APPEND _tcontent "| ------- | ------ | --- | --- | --------- |\n")
        get_property(_rows GLOBAL PROPERTY "GCC_FEATURE_TOPIC_ROWS_${_topic}")
        foreach(_row IN LISTS _rows)
            string(REPLACE "|" ";" _fields "${_row}")
            list(GET _fields 0 _bucket)
            list(GET _fields 1 _name)
            list(GET _fields 2 _std)
            list(GET _fields 3 _min_gcc)
            list(GET _fields 4 _min_lib)
            string(APPEND _tcontent
                "| [${_name}.cpp](${_bucket}/${_name}.cpp) | ${_bucket} | ${_std} | >= ${_min_gcc} | ${_min_lib} |\n")
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

    # Machine-readable inventory. Existing examples come from the same metadata
    # that drives CTest; explicit gaps live in coverage/gaps.yml so absence is
    # never mistaken for accidental omission.
    set(_ycontent "# Generated by gcc_feature_readme_indexes(); do not edit directly.\n")
    string(APPEND _ycontent "schema: 1\n")
    string(APPEND _ycontent
        "scope: 'C++11 through C++26 features supported or tracked by GCC 13 through 16'\n")
    string(APPEND _ycontent "examples:\n")
    foreach(_dir IN LISTS _dirs)
        get_property(_names GLOBAL PROPERTY "GCC_FEATURE_INDEX_${_dir}")
        list(SORT _names)
        foreach(_name IN LISTS _names)
            foreach(_field STD MIN_GCC MAX_GCC MIN_LIBSTDCXX MAX_LIBSTDCXX STATUS
                           ARCH FEATURE_MACRO PROPOSAL SKIP_REASON MODULE_INTERFACE)
                _gcc_feature_meta(_${_field} "${_name}" ${_field})
            endforeach()
            foreach(_field TAGS PREREQUISITES EXTRA_LIBS EXTRA_COMPILE_FLAGS
                           REQUIRES_SANITIZER SKIP_SANITIZER)
                _gcc_feature_meta(_${_field} "${_name}" ${_field})
            endforeach()

            file(READ "${CMAKE_SOURCE_DIR}/${_dir}/${_name}.cpp" _head LIMIT 4096)
            if(NOT _head MATCHES "// description: ([^\n]+)")
                message(FATAL_ERROR "${_dir}/${_name}.cpp: missing description")
            endif()
            set(_desc "${CMAKE_MATCH_1}")
            if(NOT _head MATCHES "// reference: ([^\n]+)")
                message(FATAL_ERROR "${_dir}/${_name}.cpp: missing reference")
            endif()
            set(_reference "${CMAKE_MATCH_1}")

            string(APPEND _ycontent "  - name: '${_name}'\n")
            string(APPEND _ycontent "    source: '${_dir}/${_name}.cpp'\n")
            string(APPEND _ycontent "    standard: '${_STD}'\n")
            string(APPEND _ycontent "    min_gcc: ${_MIN_GCC}\n")
            foreach(_pair MAX_GCC MIN_LIBSTDCXX MAX_LIBSTDCXX)
                string(TOLOWER "${_pair}" _key)
                if(_${_pair})
                    string(APPEND _ycontent "    ${_key}: ${_${_pair}}\n")
                else()
                    string(APPEND _ycontent "    ${_key}: null\n")
                endif()
            endforeach()
            string(APPEND _ycontent "    status: '${_STATUS}'\n")
            string(APPEND _ycontent "    architecture: '${_ARCH}'\n")
            string(APPEND _ycontent "    tags:\n")
            foreach(_tag IN LISTS _TAGS)
                string(APPEND _ycontent "      - '${_tag}'\n")
            endforeach()
            foreach(_pair FEATURE_MACRO PROPOSAL SKIP_REASON MODULE_INTERFACE)
                string(TOLOWER "${_pair}" _key)
                if(_${_pair})
                    _gcc_feature_yaml_quote(_value "${_${_pair}}")
                    string(APPEND _ycontent "    ${_key}: ${_value}\n")
                else()
                    string(APPEND _ycontent "    ${_key}: null\n")
                endif()
            endforeach()
            foreach(_pair PREREQUISITES EXTRA_LIBS EXTRA_COMPILE_FLAGS
                           REQUIRES_SANITIZER SKIP_SANITIZER)
                string(TOLOWER "${_pair}" _key)
                string(APPEND _ycontent "    ${_key}:")
                if(_${_pair})
                    string(APPEND _ycontent "\n")
                    foreach(_value IN LISTS _${_pair})
                        _gcc_feature_yaml_quote(_quoted "${_value}")
                        string(APPEND _ycontent "      - ${_quoted}\n")
                    endforeach()
                else()
                    string(APPEND _ycontent " []\n")
                endif()
            endforeach()
            _gcc_feature_yaml_quote(_reference_q "${_reference}")
            _gcc_feature_yaml_quote(_desc_q "${_desc}")
            string(APPEND _ycontent "    reference: ${_reference_q}\n")
            string(APPEND _ycontent "    description: ${_desc_q}\n")
        endforeach()
    endforeach()
    string(APPEND _ycontent "known_gaps:\n")
    if(EXISTS "${CMAKE_SOURCE_DIR}/coverage/gaps.yml")
        file(READ "${CMAKE_SOURCE_DIR}/coverage/gaps.yml" _gaps)
        string(APPEND _ycontent "${_gaps}")
    endif()

    set(_coverage "${CMAKE_SOURCE_DIR}/coverage.yml")
    if(GCC_FEATURE_README STREQUAL "write")
        file(WRITE "${GCC_FEATURE_README_OUT}/coverage.yml" "${_ycontent}")
    else()
        set(_coverage_existing "")
        if(EXISTS "${_coverage}")
            file(READ "${_coverage}" _coverage_existing)
        endif()
        if(NOT _coverage_existing STREQUAL _ycontent)
            list(APPEND _stale "  - coverage.yml")
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
