cmake_minimum_required(VERSION 3.8)
cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
project(${PROJ})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH})

# 检索指定目录下拥有CMakeLists.txt文件的目录
function(getCMakeListsFileDirs DirPath OutPathList)
file(GLOB directories RELATIVE ${DirPath} ${DirPath}/*)
foreach(dir ${directories})
if(IS_DIRECTORY ${DirPath}/${dir})
if(EXISTS ${DirPath}/${dir}/CMakeLists.txt)
list(APPEND ${OutPathList} ${dir})
    endif()
endif()
endforeach()
set(${OutPathList} ${${OutPathList}} PARENT_SCOPE)
endfunction()

getCMakeListsFileDirs(${CMAKE_SOURCE_DIR} RESULT)

foreach(item ${RESULT})
    add_subdirectory(${item})
endforeach(item)
