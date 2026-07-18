cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME PROJ)
set(CMAKE_C_STANDARD 11)
add_executable(${PROJ}_test main.c)
target_link_libraries(${PROJ}_test RomanNumerals)