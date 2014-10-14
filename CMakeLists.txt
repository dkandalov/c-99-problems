# based on https://github.com/dmonopoly/gtest-cmake-example/blob/master/CMakeLists.txt
cmake_minimum_required(VERSION 2.8.4)

option(test "Build all tests." ON)
set(PROJECT_NAME c_99_problems) # Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
project(${PROJECT_NAME})

# boost libs
set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.55.0_2/include/)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.55.0_2/lib)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(p99 p99.cpp)
add_executable(main main.cpp)

target_link_libraries(main p99)
target_link_libraries(main ${Boost_LIBRARIES})

if (test)
    add_subdirectory(lib/catch)
    include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})

    add_subdirectory(lib/gtest-1.7.0)
    enable_testing()
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

    add_executable(runUnitGTests p99-gtest.cpp)
    target_link_libraries(runUnitGTests gtest gtest_main)
    target_link_libraries(runUnitGTests p99)

    add_executable(runUnitTests p99-test.cpp)
    target_link_libraries(runUnitTests p99)
endif()