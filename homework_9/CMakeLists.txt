cmake_minimum_required(VERSION 3.17)
project(homework_9)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chat Chat.cpp)

set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost)
include_directories (${Boost_INCLUDE_DIRS})