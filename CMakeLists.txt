# based on https://github.com/dmonopoly/gtest-cmake-example/blob/master/CMakeLists.txt
cmake_minimum_required(VERSION 2.8.4)

option(test "Build all tests." ON)
set(PROJECT_NAME c_99_problems) # Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(p99 p99.cpp)
add_executable(main main.cpp)

target_link_libraries(main p99)

if (test)
    add_subdirectory(lib/gtest-1.7.0)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_executable(runUnitTests main-test.cpp)
    target_link_libraries(runUnitTests gtest gtest_main)
    target_link_libraries(runUnitTests p99)
endif()