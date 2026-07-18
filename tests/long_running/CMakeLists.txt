# 长时间运行的测试 - 这些程序可能需要很长时间完成
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp *.cxx *.cc)
string(JOIN ";." EXTENSIONS ${CMAKE_CXX_SOURCE_FILE_EXTENSIONS} ${CMAKE_C_SOURCE_FILE_EXTENSIONS} ${CMAKE_RC_SOURCE_FILE_EXTENSIONS})
set(EXTENSIONS ".${EXTENSIONS}")

foreach(SOURCE IN LISTS SOURCES)
    cmake_path(GET SOURCE EXTENSION LAST_ONLY EXT)
    if(EXT IN_LIST EXTENSIONS)
        cmake_path(GET SOURCE STEM PROJ)
        add_executable(${PROJ} ${SOURCE})
        
        target_link_libraries(${PROJ} PRIVATE m)
        target_include_directories(${PROJ} PRIVATE ${CMAKE_SOURCE_DIR}/src)
        
        message(STATUS "长时间运行程序: ${PROJ} (不自动测试)")
    endif()
endforeach()
