set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(testPoint testPoint.cpp)
target_link_libraries(testPoint PRIVATE planeGeometry)

add_executable(testLine testLine.cpp)
target_link_libraries(testLine PRIVATE planeGeometry)

add_executable(testEllipse testEllipse.cpp)
target_link_libraries(testEllipse PRIVATE planeGeometry)

add_executable(testCircle testCircle.cpp)
target_link_libraries(testCircle PRIVATE planeGeometry)

add_executable(testParabola testParabola.cpp)
target_link_libraries(testParabola PRIVATE planeGeometry)

add_executable(testHyperbola testHyperbola.cpp)
target_link_libraries(testHyperbola PRIVATE planeGeometry)

add_executable(testTransform2D testTransform2D.cpp)
target_link_libraries(testTransform2D PRIVATE planeGeometry)

add_executable(testConicTraits testConicTraits.cpp)
target_link_libraries(testConicTraits PRIVATE planeGeometry)

add_executable(testConicFactory testConicFactory.cpp)
target_link_libraries(testConicFactory PRIVATE planeGeometry)

add_executable(debug_parabola debug_parabola.cpp)
target_link_libraries(debug_parabola PRIVATE planeGeometry)

add_executable(testConicIntersect testConicIntersect.cpp)
target_link_libraries(testConicIntersect PRIVATE planeGeometry)

add_executable(debugIntersect debugIntersect.cpp)
target_link_libraries(debugIntersect PRIVATE planeGeometry)

add_executable(debugIntersect2 debugIntersect2.cpp)
target_link_libraries(debugIntersect2 PRIVATE planeGeometry)
