cmake_minimum_required(VERSION 3.16)
project(homework_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(Task1 Task1.cpp Timer.h)
add_executable(Task5 Task5.cpp)

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})
add_executable(Task2 Task2.cpp)
add_executable(Task3 Task3.cpp)
add_executable(Task4 Task4.cpp)