cmake_minimum_required(VERSION 3.12)
project(LAB4)

set(CMAKE_CXX_STANDARD 17)


set(Boost_NO_SYSTEM_PATHS TRUE)
if (Boost_NO_SYSTEM_PATHS)
    set(BOOST_ROOT "/usr/local/")
    set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
    set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)
find_package(Boost REQUIRED regex date_time system filesystem thread graph program_options)
include_directories(${BOOST_INCLUDE_DIRS})



add_executable(LAB4  main.cpp)
target_link_libraries(LAB4  ${Boost_LIBRARIES})