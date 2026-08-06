# Run a command, check whether it succeeds or fails, and require a matching
# diagnostic/output regex. This complements expect_failure.cmake for successful
# compiler diagnostics (for example -fanalyzer and -fopt-info-vec) and stdout.

foreach(_var EXPECT_COMMAND)
    if(NOT DEFINED ${_var} OR "${${_var}}" STREQUAL "")
        message(FATAL_ERROR "${_var} is required")
    endif()
endforeach()

if(NOT DEFINED EXPECT_COMMAND_EXIT)
    set(EXPECT_COMMAND_EXIT "zero")
endif()
if(NOT EXPECT_COMMAND_EXIT MATCHES "^(zero|nonzero)$")
    message(FATAL_ERROR "EXPECT_COMMAND_EXIT must be zero or nonzero")
endif()
if(NOT DEFINED EXPECT_COMMAND_KIND)
    set(EXPECT_COMMAND_KIND "command")
endif()

if(DEFINED EXPECT_COMMAND_WORKING_DIRECTORY)
    execute_process(
        COMMAND ${EXPECT_COMMAND}
        WORKING_DIRECTORY "${EXPECT_COMMAND_WORKING_DIRECTORY}"
        RESULT_VARIABLE _exit
        OUTPUT_VARIABLE _stdout
        ERROR_VARIABLE _stderr
    )
else()
    execute_process(
        COMMAND ${EXPECT_COMMAND}
        RESULT_VARIABLE _exit
        OUTPUT_VARIABLE _stdout
        ERROR_VARIABLE _stderr
    )
endif()

set(_output "${_stderr}\n${_stdout}")
string(ASCII 27 _esc)
string(REGEX REPLACE "${_esc}\\[[0-9;?]*[ -/]*[@-~]" "" _output "${_output}")

set(_exit_ok FALSE)
if(EXPECT_COMMAND_EXIT STREQUAL "zero" AND _exit STREQUAL "0")
    set(_exit_ok TRUE)
elseif(EXPECT_COMMAND_EXIT STREQUAL "nonzero" AND NOT _exit STREQUAL "0")
    set(_exit_ok TRUE)
endif()

if(NOT _exit_ok)
    message("exit code: ${_exit}")
    if(_stdout)
        message("---- stdout ----\n${_stdout}")
    endif()
    if(_stderr)
        message("---- stderr ----\n${_stderr}")
    endif()
    message(FATAL_ERROR
        "${EXPECT_COMMAND_KIND} exit did not satisfy '${EXPECT_COMMAND_EXIT}'"
    )
endif()

if(DEFINED EXPECT_COMMAND_REGEX AND NOT EXPECT_COMMAND_REGEX STREQUAL "")
    string(REGEX MATCH "${EXPECT_COMMAND_REGEX}" _match "${_output}")
    if(NOT _match)
        message("exit code: ${_exit}")
        if(_stdout)
            message("---- stdout ----\n${_stdout}")
        endif()
        if(_stderr)
            message("---- stderr ----\n${_stderr}")
        endif()
        message(FATAL_ERROR
            "${EXPECT_COMMAND_KIND} output did not match /${EXPECT_COMMAND_REGEX}/"
        )
    endif()
    message("${EXPECT_COMMAND_KIND} exit matched '${EXPECT_COMMAND_EXIT}' and output matched /${EXPECT_COMMAND_REGEX}/")
else()
    message("${EXPECT_COMMAND_KIND} exit matched '${EXPECT_COMMAND_EXIT}'")
endif()
