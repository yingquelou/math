cmake_minimum_required(VERSION 3.10)

add_library(strings STATIC
    kmp.c
    sunday.c
    kmp2.c
)

target_include_directories(strings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
