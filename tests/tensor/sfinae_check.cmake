# SFINAE negative test runner.
# This script invokes the compiler in syntax-only mode on the negative test file.
# - If compilation SUCCEEDS (return 0) then the SFINAE guard is NOT working:
#   we emit a FATAL_ERROR so the build fails.
# - If compilation FAILS (return non-zero) then SFINAE correctly removed the
#   operator overload and we emit a non-fatal status message.

set(NEG_TEST_SOURCE "${CMAKE_CURRENT_LIST_DIR}/test_tensor_sfinae_negative.cpp")

# Pick a compiler: prefer CMAKE_CXX_COMPILER if set, else clang++/c++/g++ fallback.
if(DEFINED CMAKE_CXX_COMPILER)
    set(COMPILER "${CMAKE_CXX_COMPILER}")
else()
    find_program(CLANGXX clang++)
    if(CLANGXX)
        set(COMPILER "${CLANGXX}")
    else()
        find_program(CPP c++)
        if(CPP)
            set(COMPILER "${CPP}")
        else()
            find_program(GXX g++)
            if(GXX)
                set(COMPILER "${GXX}")
            else()
                set(COMPILER "clang++")
            endif()
        endif()
    endif()
endif()

execute_process(
    COMMAND "${COMPILER}" -std=c++11 -I${CMAKE_CURRENT_SOURCE_DIR} -fsyntax-only "${NEG_TEST_SOURCE}"
    RESULT_VARIABLE COMPILE_RESULT
    OUTPUT_VARIABLE COMPILE_OUT
    ERROR_VARIABLE COMPILE_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)

if(COMPILE_RESULT EQUAL 0)
    message(FATAL_ERROR
        "SFINAE NEGATIVE TEST FAILED: operator+ should not compile for Tensor<NoNum,2>. "
        "Compilation succeeded unexpectedly. (compiler output: ${COMPILE_OUT} ${COMPILE_ERR})")
else()
    message(STATUS "SFINAE NEGATIVE TEST PASSED (compilation correctly rejected; compiler returned non-zero)")
endif()

# Write a marker file so CMake's custom_command target can depend on it.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/sfinae_check_marker" "ok\n")
