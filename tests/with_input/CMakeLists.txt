# Tests that require input parameters
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp *.cxx *.cc)
string(JOIN ";." EXTENSIONS ${CMAKE_CXX_SOURCE_FILE_EXTENSIONS} ${CMAKE_C_SOURCE_FILE_EXTENSIONS} ${CMAKE_RC_SOURCE_FILE_EXTENSIONS})
set(EXTENSIONS ".${EXTENSIONS}")

set(TEST_INPUTS
    gcd_recursive:"10 15"
    gcd_subtraction:"20 30"
    gcd_loop:"25 35"
    prime_range:"10 20"
    prime_range_finder:"1 100"
)

foreach(SOURCE IN LISTS SOURCES)
    cmake_path(GET SOURCE EXTENSION LAST_ONLY EXT)
    if(EXT IN_LIST EXTENSIONS)
        cmake_path(GET SOURCE STEM PROJ)
        add_executable(${PROJ} ${SOURCE})
        target_link_libraries(${PROJ} PRIVATE gcd prime)
        target_include_directories(${PROJ} PRIVATE ${CMAKE_SOURCE_DIR}/src)
        
        set(TEST_INPUT "")
        foreach(TEST_INPUT_PAIR IN LISTS TEST_INPUTS)
            string(REGEX MATCH "^${PROJ}:(.+)$" MATCHED "${TEST_INPUT_PAIR}")
            if(MATCHED)
                set(TEST_INPUT "${CMAKE_MATCH_1}")
                break()
            endif()
        endforeach()
        
        if(TEST_INPUT)
            set(TEST_INPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJ}_input.txt")
            file(WRITE ${TEST_INPUT_FILE} "${TEST_INPUT}")
            
            if(WIN32)
                message(STATUS "Configure test input for ${PROJ}: ${TEST_INPUT}")
                message(STATUS "  Input file: ${TEST_INPUT_FILE}")
            else()
                add_test(NAME ${PROJ} COMMAND sh -c "$<TARGET_FILE:${PROJ}> < ${TEST_INPUT_FILE}")
                message(STATUS "Configure test input for ${PROJ}: ${TEST_INPUT}")
            endif()
        endif()
    endif()
endforeach()