cmake_minimum_required(VERSION 3.5)
project(School)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES tut.cpp School.cpp School.h)
add_executable(School ${SOURCE_FILES} School.cpp School.h)