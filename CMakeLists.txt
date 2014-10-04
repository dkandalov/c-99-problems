cmake_minimum_required(VERSION 2.8.4)
project(c_99_problems)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(c_99_problems ${SOURCE_FILES})