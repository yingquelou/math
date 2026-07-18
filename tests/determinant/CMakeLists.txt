set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(determinant_2_3 determinant_2_3.c)
target_link_libraries(determinant_2_3 PRIVATE determinant m)
add_test(NAME determinant_2_3 COMMAND $<TARGET_FILE:determinant_2_3>)

add_executable(determinant_n determinant_n.c)
target_link_libraries(determinant_n PRIVATE determinant m)
add_test(NAME determinant_n COMMAND $<TARGET_FILE:determinant_n>)