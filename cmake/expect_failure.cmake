# Run a command that is expected to fail and require a matching diagnostic.

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
    message("${EXPECT_FAILURE_KIND} was expected to fail, but succeeded")
    if(_stdout)
        message("---- stdout ----")
        message("${_stdout}")
    endif()
    if(_stderr)
        message("---- stderr ----")
        message("${_stderr}")
    endif()
    cmake_language(EXIT 1)
endif()

string(REGEX MATCH "${EXPECT_FAILURE_REGEX}" _match "${_output}")
if(_match)
    message("${EXPECT_FAILURE_KIND} failed as expected and matched /${EXPECT_FAILURE_REGEX}/")
    cmake_language(EXIT 0)
endif()

message("${EXPECT_FAILURE_KIND} failed, but output did not match /${EXPECT_FAILURE_REGEX}/")
message("exit code: ${_exit}")
if(_stdout)
    message("---- stdout ----")
    message("${_stdout}")
endif()
if(_stderr)
    message("---- stderr ----")
    message("${_stderr}")
endif()
cmake_language(EXIT 1)
