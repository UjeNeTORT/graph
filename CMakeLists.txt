cmake_minimum_required(VERSION 3.9)
project(graph LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g -O0 -fsanitize=address")
set (CMAKE_CXX_FLAGS_RELEASE "-O2")
set (CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_library(GTEST_LIBRARY NAMES gtest gtest_main)

add_subdirectory(CXXGraph)
add_subdirectory(cxxopts)
add_subdirectory(ujenegraph)

add_executable(graph main.cpp)
target_link_libraries(graph PRIVATE ujenegraph CXXGraph)
target_include_directories(graph PRIVATE ${CMAKE_SOURCE_DIR}/ujenegraph
                                         ${CMAKE_SOURCE_DIR}/cxxopts/include
                                         ${CMAKE_SOURCE_DIR}/CXXGraph/include)
