cmake_minimum_required(VERSION 3.8)

file(GLOB SRCLIST1 *.cpp)

foreach(ITEM ${SRCLIST1})
cmake_path(GET ITEM STEM CURFILENAME)
project(${CURFILENAME} LANGUAGES CXX)
add_executable(${CURFILENAME} ${ITEM})
endforeach(ITEM)

file(GLOB SRCLIST2 *.c)

foreach(ITEM ${SRCLIST2})
cmake_path(GET ITEM STEM CURFILENAME)
project(${CURFILENAME} LANGUAGES C)
add_executable(${CURFILENAME} ${ITEM})
endforeach(ITEM)