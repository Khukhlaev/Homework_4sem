cmake_minimum_required(VERSION 3.17)
project(homework_8)

set(CMAKE_CXX_STANDARD 20)

add_executable(Task1 Task1.cpp)
add_executable(Task2 Task2.cpp)
add_executable(Task3 Task3.cpp)
add_executable(Task4 Task4.cpp)

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})