set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tensor_test test_tensor.cpp)
target_link_libraries(tensor_test PRIVATE tensor)

set(SFINAE_MARKER "${CMAKE_CURRENT_BINARY_DIR}/sfinae_check_marker")

add_custom_command(
    OUTPUT  "${SFINAE_MARKER}"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/sfinae_check.cmake"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test_tensor_sfinae_negative.cpp"
    COMMENT "Running SFINAE negative test (expected to fail compilation)"
    VERBATIM
)

add_custom_target(tensor_sfinae_negative_check ALL DEPENDS "${SFINAE_MARKER}")
add_dependencies(tensor_test tensor_sfinae_negative_check)
