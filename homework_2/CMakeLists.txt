cmake_minimum_required(VERSION 3.16)
project(homework_2)

set(CMAKE_CXX_STANDARD 17)


add_executable(Vector Vector.cpp)
add_executable(Sort Sort.cpp Timer.h)

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})
add_executable(vector_to_multiarray_raw vector_to_multiarray_raw.cpp)