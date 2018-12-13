cmake_minimum_required(VERSION 3.12)
project(LAB9_Prob2)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_COMPILER mpicxx)
add_executable(LAB9_Prob2 main.cpp)
target_link_libraries(LAB9_Prob2  ${Boost_LIBRARIES})