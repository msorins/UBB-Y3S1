cmake_minimum_required(VERSION 3.12)
project(DSM_Library)

set(CMAKE_CXX_STANDARD 14)

add_library(DSM_Library SHARED library.cpp library.h)