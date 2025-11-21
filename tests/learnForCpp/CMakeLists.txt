file(GLOB SRCLIST *.cpp)

foreach(ITEM ${SRCLIST})
cmake_path(GET ITEM STEM CURFILENAME)
add_executable(${CURFILENAME} ${ITEM})
endforeach(ITEM)
