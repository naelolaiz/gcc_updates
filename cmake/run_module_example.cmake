# Compile a module interface before its importer, link them, then run the demo.
# Kept as a CTest fixture because CMake 3.20 predates native C++ module support.

foreach(_var MODULE_CXX MODULE_FLAGS MODULE_INCLUDE MODULE_INTERFACE MODULE_IMPORTER MODULE_WORK)
    if(NOT DEFINED ${_var} OR "${${_var}}" STREQUAL "")
        message(FATAL_ERROR "${_var} is required")
    endif()
endforeach()

file(REMOVE_RECURSE "${MODULE_WORK}")
file(MAKE_DIRECTORY "${MODULE_WORK}")

execute_process(
    COMMAND "${MODULE_CXX}" ${MODULE_FLAGS} "-I${MODULE_INCLUDE}"
            -x c++ -c "${MODULE_INTERFACE}" -o module-interface.o
    WORKING_DIRECTORY "${MODULE_WORK}"
    RESULT_VARIABLE _interface_exit
    OUTPUT_VARIABLE _interface_stdout
    ERROR_VARIABLE _interface_stderr
)
if(NOT _interface_exit STREQUAL "0")
    message(FATAL_ERROR
        "module interface compilation failed (${_interface_exit})\n"
        "${_interface_stdout}\n${_interface_stderr}"
    )
endif()

execute_process(
    COMMAND "${MODULE_CXX}" ${MODULE_FLAGS} "-I${MODULE_INCLUDE}"
            "${MODULE_IMPORTER}" module-interface.o -o module-demo
    WORKING_DIRECTORY "${MODULE_WORK}"
    RESULT_VARIABLE _importer_exit
    OUTPUT_VARIABLE _importer_stdout
    ERROR_VARIABLE _importer_stderr
)
if(NOT _importer_exit STREQUAL "0")
    message(FATAL_ERROR
        "module importer compilation failed (${_importer_exit})\n"
        "${_importer_stdout}\n${_importer_stderr}"
    )
endif()

execute_process(
    COMMAND "${MODULE_WORK}/module-demo"
    WORKING_DIRECTORY "${MODULE_WORK}"
    RESULT_VARIABLE _run_exit
    OUTPUT_VARIABLE _run_stdout
    ERROR_VARIABLE _run_stderr
)
if(NOT _run_exit STREQUAL "0")
    message(FATAL_ERROR
        "module demo failed (${_run_exit})\n${_run_stdout}\n${_run_stderr}"
    )
endif()

message("module interface, importer, link, and runtime checks passed")
