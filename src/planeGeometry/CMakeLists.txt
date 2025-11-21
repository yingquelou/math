add_library(Ellipse Ellipse.cpp)

add_library(Line Line.cpp)
target_link_libraries(Line Point)

add_library(Point Point.cpp)
add_executable(testPoint testPoint.cpp)
target_link_libraries(testPoint Point)