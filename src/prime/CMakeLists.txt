add_library(prime STATIC
    prime.c
)

target_include_directories(prime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
