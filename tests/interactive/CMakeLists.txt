# 交互式输入测试 - 这些程序需要无限循环的交互输入，不自动添加测试
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp *.cxx *.cc)
string(JOIN ";." EXTENSIONS ${CMAKE_CXX_SOURCE_FILE_EXTENSIONS} ${CMAKE_C_SOURCE_FILE_EXTENSIONS} ${CMAKE_RC_SOURCE_FILE_EXTENSIONS})
set(EXTENSIONS ".${EXTENSIONS}")

foreach(SOURCE IN LISTS SOURCES)
    cmake_path(GET SOURCE EXTENSION LAST_ONLY EXT)
    if(EXT IN_LIST EXTENSIONS)
        cmake_path(GET SOURCE STEM PROJ)
        add_executable(${PROJ} ${SOURCE})
        
        target_include_directories(${PROJ} PRIVATE ${CMAKE_SOURCE_DIR}/src)
        
        if(${PROJ} STREQUAL "combinations")
            target_link_libraries(${PROJ} PRIVATE combinatorics)
        else()
            target_link_libraries(${PROJ} PRIVATE prime)
        endif()
        
        message(STATUS "交互式程序: ${PROJ} (不自动测试)")
    endif()
endforeach()
