# Run a command that is expected to fail and require a matching diagnostic.
# Exit protocol: falling off the end of the script exits 0 (test passes);
# any unmet expectation uses message(FATAL_ERROR), which exits non-zero.
# Deliberately avoids cmake_language(EXIT): that needs CMake 3.29, newer
# than this project's 3.20 minimum (the gcc:13 image ships 3.25).

foreach(_var EXPECT_FAILURE_COMMAND EXPECT_FAILURE_REGEX)
    if(NOT DEFINED ${_var} OR "${${_var}}" STREQUAL "")
        message(FATAL_ERROR "${_var} is required")
    endif()
endforeach()

if(NOT DEFINED EXPECT_FAILURE_KIND)
    set(EXPECT_FAILURE_KIND "command")
endif()

if(DEFINED EXPECT_FAILURE_WORKING_DIRECTORY)
    execute_process(
        COMMAND ${EXPECT_FAILURE_COMMAND}
        WORKING_DIRECTORY "${EXPECT_FAILURE_WORKING_DIRECTORY}"
        RESULT_VARIABLE _exit
        OUTPUT_VARIABLE _stdout
        ERROR_VARIABLE  _stderr
    )
else()
    execute_process(
        COMMAND ${EXPECT_FAILURE_COMMAND}
        RESULT_VARIABLE _exit
        OUTPUT_VARIABLE _stdout
        ERROR_VARIABLE  _stderr
    )
endif()

set(_output "${_stderr}\n${_stdout}")
string(ASCII 27 _esc)
string(REGEX REPLACE "${_esc}\\[[0-9;?]*[ -/]*[@-~]" "" _output "${_output}")

if(_exit STREQUAL "0")
    if(_stdout)
        message("---- stdout ----")
        message("${_stdout}")
    endif()
    if(_stderr)
        message("---- stderr ----")
        message("${_stderr}")
    endif()
    message(FATAL_ERROR "${EXPECT_FAILURE_KIND} was expected to fail, but succeeded")
endif()

string(REGEX MATCH "${EXPECT_FAILURE_REGEX}" _match "${_output}")
if(NOT _match)
    message("exit code: ${_exit}")
    if(_stdout)
        message("---- stdout ----")
        message("${_stdout}")
    endif()
    if(_stderr)
        message("---- stderr ----")
        message("${_stderr}")
    endif()
    message(FATAL_ERROR "${EXPECT_FAILURE_KIND} failed, but output did not match /${EXPECT_FAILURE_REGEX}/")
endif()

message("${EXPECT_FAILURE_KIND} failed as expected and matched /${EXPECT_FAILURE_REGEX}/")
