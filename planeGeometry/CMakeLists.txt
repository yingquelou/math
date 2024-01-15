cmake_minimum_required(VERSION 3.8)

# project(Ellipse LANGUAGES CXX)
# add_library(Ellipse Ellipse.cpp)

add_library(Line SHARED Line.cpp)
target_link_libraries(Line Point)

add_library(Point SHARED Point.cpp)
add_executable(testPoint testPoint.cpp)
target_link_libraries(testPoint Point)
