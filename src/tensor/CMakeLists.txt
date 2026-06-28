project(tensor_lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tensor INTERFACE)
target_include_directories(tensor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
