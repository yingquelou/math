add_library(combinatorics STATIC
    combinatorics.c
)

target_include_directories(combinatorics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})