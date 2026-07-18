add_library(gcd STATIC
    gcd.c
)

target_include_directories(gcd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})