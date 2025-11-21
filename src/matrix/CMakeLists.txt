cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM PROJ)
set(CMAKE_CXX_STANDARD 20)
add_executable(${PROJ} Matrix.cpp MatrixMain.cpp)