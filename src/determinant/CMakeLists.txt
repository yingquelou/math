add_library(determinant STATIC
    determinant.c
)

target_include_directories(determinant PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})