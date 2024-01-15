cmake_minimum_required(VERSION 3.8)

cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM PROJ)
add_executable(${PROJ} Matrix.cpp MatrixMain.cpp)
message(${PROJ})